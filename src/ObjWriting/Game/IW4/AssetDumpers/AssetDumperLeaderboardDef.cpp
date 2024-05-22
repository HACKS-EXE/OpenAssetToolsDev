#include "AssetDumperLeaderboardDef.h"

#include "Game/IW4/Leaderboard/JsonLeaderboardDefWriter.h"

#include <format>
#include <ranges>

using namespace IW4;

std::string AssetDumperLeaderboardDef::GetFileNameForAsset(const std::string& assetName)
{

    return std::format("leaderboards/{}.json", assetName);
}

bool AssetDumperLeaderboardDef::ShouldDump(XAssetInfo<LeaderboardDef>* asset)
{
    return true;
}

void AssetDumperLeaderboardDef::DumpAsset(AssetDumpingContext& context, XAssetInfo<LeaderboardDef>* asset)
{
    const auto assetFile = context.OpenAssetFile(GetFileNameForAsset(asset->m_name));

    if (!assetFile)
        return;

    DumpLeaderboardDefAsJson(*assetFile, asset->Asset());
}
