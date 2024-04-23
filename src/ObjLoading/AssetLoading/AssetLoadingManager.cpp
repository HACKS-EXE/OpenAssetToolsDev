#include "AssetLoadingManager.h"

#include "Utils/StringUtils.h"

#include <algorithm>
#include <format>
#include <iostream>

AssetLoadingManager::AssetLoadingManager(const std::map<asset_type_t, std::unique_ptr<IAssetLoader>>& assetLoadersByType, AssetLoadingContext& context)
    : m_asset_loaders_by_type(assetLoadersByType),
      m_context(context),
      m_last_dependency_loaded(nullptr)
{
}

bool AssetLoadingManager::LoadAssetFromLoader(const asset_type_t assetType, const std::string& assetName)
{
    return LoadDependency(assetType, assetName) != nullptr;
}

AssetLoadingContext* AssetLoadingManager::GetAssetLoadingContext() const
{
    return &m_context;
}

XAssetInfoGeneric* AssetLoadingManager::AddAssetInternal(std::unique_ptr<XAssetInfoGeneric> xAssetInfo)
{
    const auto assetType = xAssetInfo->m_type;
    const auto* pAssetName = xAssetInfo->m_name.c_str();

    m_last_dependency_loaded = m_context.m_zone->m_pools->AddAsset(std::move(xAssetInfo));
    if (m_last_dependency_loaded == nullptr)
        std::cerr << "Failed to add asset of type \"" << m_context.m_zone->m_pools->GetAssetTypeName(assetType) << "\" to pool: \"" << pAssetName << "\"\n";

    return m_last_dependency_loaded;
}

XAssetInfoGeneric* AssetLoadingManager::AddAsset(std::unique_ptr<XAssetInfoGeneric> xAssetInfo)
{
    xAssetInfo->m_zone = m_context.m_zone;
    return AddAssetInternal(std::move(xAssetInfo));
}

XAssetInfoGeneric* AssetLoadingManager::LoadIgnoredDependency(const asset_type_t assetType, const std::string& assetName, IAssetLoader* loader)
{
    auto* alreadyLoadedAsset = m_context.m_zone->m_pools->GetAssetOrAssetReference(assetType, assetName);
    if (alreadyLoadedAsset)
        return alreadyLoadedAsset;

    auto* linkAsset = loader->CreateEmptyAsset(assetName, m_context.m_zone->GetMemory());
    if (linkAsset)
    {
        AddAsset(std::make_unique<XAssetInfoGeneric>(assetType, assetName, linkAsset));
        auto* lastDependency = m_last_dependency_loaded;
        m_last_dependency_loaded = nullptr;
        return lastDependency;
    }

    auto* existingAsset = loader->LoadFromGlobalAssetPools(assetName);
    if (existingAsset)
    {
        AddAssetInternal(std::make_unique<XAssetInfoGeneric>(*existingAsset));
        auto* lastDependency = m_last_dependency_loaded;
        m_last_dependency_loaded = nullptr;
        return lastDependency;
    }

    std::cerr << "Failed to create empty asset \"" << assetName << "\" for type \"" << m_context.m_zone->m_pools->GetAssetTypeName(assetType) << "\"\n";
    return nullptr;
}

XAssetInfoGeneric* AssetLoadingManager::LoadAssetDependency(const asset_type_t assetType, const std::string& assetName, const IAssetLoader* loader)
{
    if (loader->CanLoadFromGdt() && !m_context.m_gdt_files.empty()
        && loader->LoadFromGdt(assetName, &m_context, m_context.m_zone->GetMemory(), this, m_context.m_zone))
    {
        auto* lastDependency = m_last_dependency_loaded;
        m_last_dependency_loaded = nullptr;
        return lastDependency;
    }

    if (loader->CanLoadFromRaw() && loader->LoadFromRaw(assetName, m_context.m_raw_search_path, m_context.m_zone->GetMemory(), this, m_context.m_zone))
    {
        auto* lastDependency = m_last_dependency_loaded;
        m_last_dependency_loaded = nullptr;
        return lastDependency;
    }

    auto* existingAsset = loader->LoadFromGlobalAssetPools(assetName);
    if (!existingAsset && !assetName.empty() && assetName[0] != ',')
        existingAsset = loader->LoadFromGlobalAssetPools(',' + assetName);

    if (existingAsset)
    {
        std::vector<XAssetInfoGeneric*> dependencies;
        std::vector<IndirectAssetReference> indirectAssetReferences;
        for (const auto* dependency : existingAsset->m_dependencies)
        {
            auto* newDependency = LoadDependency(dependency->m_type, dependency->m_name);
            if (newDependency)
                dependencies.push_back(newDependency);
            else
                return nullptr;
        }

        indirectAssetReferences.reserve(existingAsset->m_indirect_asset_references.size());
        for (const auto& indirectAssetReference : existingAsset->m_indirect_asset_references)
            indirectAssetReferences.emplace_back(LoadIndirectAssetReference(indirectAssetReference.m_type, indirectAssetReference.m_name));

        // Make sure any used script string is available in the created zone
        // The replacement of the scr_string_t values will be done upon writing
        for (const auto scrString : existingAsset->m_used_script_strings)
            m_context.m_zone->m_script_strings.AddOrGetScriptString(existingAsset->m_zone->m_script_strings.CValue(scrString));

        AddAssetInternal(std::make_unique<XAssetInfoGeneric>(existingAsset->m_type,
                                                             existingAsset->m_name,
                                                             existingAsset->m_ptr,
                                                             std::move(dependencies),
                                                             existingAsset->m_used_script_strings,
                                                             std::move(indirectAssetReferences),
                                                             existingAsset->m_zone));

        auto* lastDependency = m_last_dependency_loaded;
        m_last_dependency_loaded = nullptr;
        return lastDependency;
    }

    std::cerr << "Failed to load asset of type \"" << m_context.m_zone->m_pools->GetAssetTypeName(assetType) << "\": \"" << assetName << "\"\n";
    return nullptr;
}

XAssetInfoGeneric* AssetLoadingManager::LoadDependency(const asset_type_t assetType, const std::string& assetName)
{
    const auto normalizedAssetName = NormalizeAssetName(assetName);

    auto* alreadyLoadedAsset = m_context.m_zone->m_pools->GetAssetOrAssetReference(assetType, normalizedAssetName);
    if (alreadyLoadedAsset)
        return alreadyLoadedAsset;

    const auto loader = m_asset_loaders_by_type.find(assetType);
    if (loader != m_asset_loaders_by_type.end())
    {
        const auto ignoreEntry = m_context.m_ignored_asset_map.find(normalizedAssetName);
        if (ignoreEntry != m_context.m_ignored_asset_map.end() && ignoreEntry->second == assetType)
        {
            const auto linkAssetName = std::format(",{}", normalizedAssetName);

            return LoadIgnoredDependency(assetType, linkAssetName, loader->second.get());
        }

        return LoadAssetDependency(assetType, normalizedAssetName, loader->second.get());
    }

    std::cerr << "Failed to find loader for asset type \"" << m_context.m_zone->m_pools->GetAssetTypeName(assetType) << "\"\n";
    return nullptr;
}

IndirectAssetReference AssetLoadingManager::LoadIndirectAssetReference(const asset_type_t assetType, const std::string& assetName)
{
    const auto normalizedAssetName = NormalizeAssetName(assetName);

    const auto* alreadyLoadedAsset = m_context.m_zone->m_pools->GetAssetOrAssetReference(assetType, normalizedAssetName);
    if (alreadyLoadedAsset)
        return IndirectAssetReference(assetType, normalizedAssetName);

    const auto ignoreEntry = m_context.m_ignored_asset_map.find(normalizedAssetName);
    if (ignoreEntry != m_context.m_ignored_asset_map.end() && ignoreEntry->second == assetType)
        return IndirectAssetReference(assetType, normalizedAssetName);

    const auto loader = m_asset_loaders_by_type.find(assetType);
    if (loader != m_asset_loaders_by_type.end())
    {
        LoadAssetDependency(assetType, normalizedAssetName, loader->second.get());
        return IndirectAssetReference(assetType, normalizedAssetName);
    }

    std::cerr << "Failed to find loader for asset type \"" << m_context.m_zone->m_pools->GetAssetTypeName(assetType) << "\"\n";
    return IndirectAssetReference(assetType, normalizedAssetName);
}

std::string AssetLoadingManager::NormalizeAssetName(const std::string& assetName)
{
    std::string result(assetName);
    utils::MakeStringLowerCase(result);

    std::ranges::replace(result, '\\', '/');

    return result;
}
