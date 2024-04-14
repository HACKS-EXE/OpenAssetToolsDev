#include "ZoneAssetPools.h"

#include <format>

ZoneAssetPools::ZoneAssetPools(Zone* zone)
    : m_zone(zone)
{
}

XAssetInfoGeneric* ZoneAssetPools::AddAsset(const asset_type_t type,
                                            std::string name,
                                            void* asset,
                                            std::vector<XAssetInfoGeneric*> dependencies,
                                            std::vector<scr_string_t> usedScriptStrings,
                                            std::vector<IndirectAssetReference> indirectAssetReferences)
{
    return AddAsset(std::make_unique<XAssetInfoGeneric>(
        type, std::move(name), asset, std::move(dependencies), std::move(usedScriptStrings), std::move(indirectAssetReferences), m_zone));
}

XAssetInfoGeneric* ZoneAssetPools::AddAsset(std::unique_ptr<XAssetInfoGeneric> xAssetInfo)
{
    auto* assetInfo = AddAssetToPool(std::move(xAssetInfo));
    if (assetInfo)
        m_assets_in_order.push_back(assetInfo);

    return assetInfo;
}

XAssetInfoGeneric* ZoneAssetPools::GetAssetOrAssetReference(const asset_type_t type, const std::string& name) const
{
    auto* result = GetAsset(type, name);

    if (result != nullptr || (!name.empty() && name[0] == ','))
        return result;

    result = GetAsset(type, std::format(",{}", name));
    return result;
}

size_t ZoneAssetPools::GetTotalAssetCount() const
{
    return m_assets_in_order.size();
}

ZoneAssetPools::iterator ZoneAssetPools::begin() const
{
    return m_assets_in_order.begin();
}

ZoneAssetPools::iterator ZoneAssetPools::end() const
{
    return m_assets_in_order.end();
}
