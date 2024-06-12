#pragma once
#include "AssetLoading/BasicAssetLoader.h"
#include "AssetLoading/IAssetLoadingManager.h"
#include "Game/T5/T5.h"
#include "SearchPath/ISearchPath.h"

namespace T5
{
    class AssetLoaderRawFile final : public BasicAssetLoader<AssetRawFile>
    {
        static constexpr size_t COMPRESSED_BUFFER_SIZE_PADDING = 64;

        static bool LoadGsc(
            const SearchPathOpenFile& file, const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager);
        static bool LoadDefault(
            const SearchPathOpenFile& file, const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager);

    public:
        _NODISCARD void* CreateEmptyAsset(const std::string& assetName, MemoryManager* memory) override;
        _NODISCARD bool CanLoadFromRaw() const override;
        bool
            LoadFromRaw(const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
    };
} // namespace T5
