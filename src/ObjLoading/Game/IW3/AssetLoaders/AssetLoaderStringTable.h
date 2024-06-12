#pragma once

#include "AssetLoading/BasicAssetLoader.h"
#include "Game/IW3/IW3.h"
#include "SearchPath/ISearchPath.h"

namespace IW3
{
    class AssetLoaderStringTable final : public BasicAssetLoader<AssetStringTable>
    {
    public:
        _NODISCARD void* CreateEmptyAsset(const std::string& assetName, MemoryManager* memory) override;
        _NODISCARD bool CanLoadFromRaw() const override;
        bool
            LoadFromRaw(const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
    };
} // namespace IW3
