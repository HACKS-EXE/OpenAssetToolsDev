#pragma once
#include "AssetLoading/BasicAssetLoader.h"
#include "AssetLoading/IAssetLoadingManager.h"
#include "Game/T6/T6.h"
#include "InfoString/InfoString.h"
#include "SearchPath/ISearchPath.h"

namespace T6
{
    class AssetLoaderWeapon final : public BasicAssetLoader<ASSET_TYPE_WEAPON, WeaponVariantDef>
    {
        static void LinkWeaponFullDefSubStructs(WeaponFullDef* weapon);

        static bool IsStringOverride(const char* baseString, const char* overrideString);
        static bool IsFxOverride(const FxEffectDef* baseEffect, const FxEffectDef* overrideEffect);
        static void
            HandleSoundOverride(WeaponAttachmentUnique* attachmentUnique, const char* snd1, const char* snd2, eAttachmentOverrideSounds sndOverrideIndex);
        static void HandleFxOverride(WeaponAttachmentUnique* attachmentUnique,
                                     const FxEffectDef* effect1,
                                     const FxEffectDef* effect2,
                                     eAttachmentOverrideEffects fxOverrideIndex);

        static void CalculateWeaponFields(WeaponFullDef* weapon);
        static void CalculateAttachmentFields(const WeaponFullDef* weapon, unsigned attachmentIndex, WeaponAttachmentUnique* attachmentUnique);
        static void CalculateAttachmentFields(const WeaponFullDef* weapon);

        static bool
            LoadFromInfoString(const InfoString& infoString, const std::string& assetName, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone);

    public:
        _NODISCARD void* CreateEmptyAsset(const std::string& assetName, MemoryManager* memory) override;
        _NODISCARD bool CanLoadFromGdt() const override;
        bool LoadFromGdt(
            const std::string& assetName, IGdtQueryable* gdtQueryable, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
        _NODISCARD bool CanLoadFromRaw() const override;
        bool
            LoadFromRaw(const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const override;
    };
} // namespace T6
