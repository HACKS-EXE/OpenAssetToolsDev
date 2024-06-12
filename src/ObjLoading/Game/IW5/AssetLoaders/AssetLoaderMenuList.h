#pragma once

#include "AssetLoading/BasicAssetLoader.h"
#include "AssetLoading/IAssetLoadingManager.h"
#include "Game/IW5/IW5.h"
#include "SearchPath/ISearchPath.h"

namespace IW5
{
    class AssetLoaderMenuList final : public BasicAssetLoader<AssetMenuList>
    {
    public:
        _NODISCARD void* CreateEmptyAsset(const std::string& assetName, MemoryManager* memory) override;
        _NODISCARD bool CanLoadFromRaw() const override;
        bool
            LoadFromRaw(const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
        void FinalizeAssetsForZone(AssetLoadingContext* context) const override;
    };
} // namespace IW5
