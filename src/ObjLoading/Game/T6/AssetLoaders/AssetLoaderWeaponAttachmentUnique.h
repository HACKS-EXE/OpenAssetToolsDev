#pragma once
#include "AssetLoading/BasicAssetLoader.h"
#include "AssetLoading/IAssetLoadingManager.h"
#include "Game/T6/T6.h"
#include "InfoString/InfoString.h"
#include "SearchPath/ISearchPath.h"

namespace T6
{
    class AssetLoaderWeaponAttachmentUnique final : public BasicAssetLoader<AssetAttachmentUnique>
    {
        static void LinkAttachmentUniqueFullSubStructs(WeaponAttachmentUniqueFull* attachmentUnique);
        static bool CalculateAttachmentUniqueFields(const std::string& assetName, WeaponAttachmentUniqueFull* attachmentUnique);

        static bool
            LoadFromInfoString(const InfoString& infoString, const std::string& assetName, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone);

    public:
        static bool ExtractAttachmentsFromAssetName(const std::string& assetName, std::vector<eAttachment>& attachmentList);

        _NODISCARD void* CreateEmptyAsset(const std::string& assetName, MemoryManager* memory) override;
        _NODISCARD bool CanLoadFromGdt() const override;
        bool LoadFromGdt(
            const std::string& assetName, IGdtQueryable* gdtQueryable, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
        _NODISCARD bool CanLoadFromRaw() const override;
        bool
            LoadFromRaw(const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
    };
} // namespace T6
