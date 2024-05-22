#include "AssetLoaderWeaponAttachment.h"

#include "Game/T6/InfoString/EnumStrings.h"
#include "Game/T6/InfoString/InfoStringToStructConverter.h"
#include "Game/T6/InfoString/WeaponAttachmentFields.h"
#include "Game/T6/ObjConstantsT6.h"
#include "Game/T6/T6.h"
#include "InfoString/InfoString.h"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace T6;

namespace T6
{
    eAttachmentPoint attachmentPointByAttachmentTable[]{
        ATTACHMENT_POINT_NONE,    // none
        ATTACHMENT_POINT_TOP,     // acog
        ATTACHMENT_POINT_TRIGGER, // dualclip
        ATTACHMENT_POINT_TOP,     // dualoptic
        ATTACHMENT_POINT_BOTTOM,  // dw
        ATTACHMENT_POINT_MUZZLE,  // extbarrel
        ATTACHMENT_POINT_TRIGGER, // extclip
        ATTACHMENT_POINT_TRIGGER, // extramags
        ATTACHMENT_POINT_GUNPERK, // fastads
        ATTACHMENT_POINT_TOP,     // fastreload
        ATTACHMENT_POINT_TRIGGER, // fmj
        ATTACHMENT_POINT_BOTTOM,  // gl
        ATTACHMENT_POINT_BOTTOM,  // grip
        ATTACHMENT_POINT_TOP,     // holo
        ATTACHMENT_POINT_BOTTOM,  // ir
        ATTACHMENT_POINT_BOTTOM,  // is
        ATTACHMENT_POINT_GUNPERK, // longbreath
        ATTACHMENT_POINT_BOTTOM,  // mk
        ATTACHMENT_POINT_TOP,     // mms
        ATTACHMENT_POINT_TOP,     // rangefinder
        ATTACHMENT_POINT_TOP,     // reflex
        ATTACHMENT_POINT_MUZZLE,  // rf
        ATTACHMENT_POINT_BOTTOM,  // sf
        ATTACHMENT_POINT_MUZZLE,  // silencer
        ATTACHMENT_POINT_TRIGGER, // stackfire
        ATTACHMENT_POINT_GUNPERK, // stalker
        ATTACHMENT_POINT_GUNPERK, // steadyaim
        ATTACHMENT_POINT_GUNPERK, // swayreduc
        ATTACHMENT_POINT_TOP,     // tacknife
        ATTACHMENT_POINT_TOP,     // vzoom
    };

    static_assert(std::extent_v<decltype(attachmentPointByAttachmentTable)> == ATTACHMENT_TYPE_COUNT);

    class InfoStringToWeaponAttachmentConverter final : public InfoStringToStructConverter
    {
    protected:
        bool ConvertExtensionField(const cspField_t& field, const std::string& value) override
        {
            switch (static_cast<attachmentFieldType_t>(field.iFieldType))
            {
            case AFT_ATTACHMENTTYPE:
                return ConvertEnumInt(field.szName, value, field.iOffset, szAttachmentTypeNames, std::extent_v<decltype(szAttachmentTypeNames)>);

            case AFT_PENETRATE_TYPE:
                return ConvertEnumInt(field.szName, value, field.iOffset, penetrateTypeNames, std::extent_v<decltype(penetrateTypeNames)>);

            case AFT_FIRETYPE:
                return ConvertEnumInt(field.szName, value, field.iOffset, szWeapFireTypeNames, std::extent_v<decltype(szWeapFireTypeNames)>);

            default:
                assert(false);
                return false;
            }
        }

    public:
        InfoStringToWeaponAttachmentConverter(const InfoString& infoString,
                                              WeaponAttachment* weaponAttachment,
                                              ZoneScriptStrings& zoneScriptStrings,
                                              MemoryManager* memory,
                                              IAssetLoadingManager* manager,
                                              const cspField_t* fields,
                                              const size_t fieldCount)
            : InfoStringToStructConverter(infoString, weaponAttachment, zoneScriptStrings, memory, manager, fields, fieldCount)
        {
        }
    };
} // namespace T6

void AssetLoaderWeaponAttachment::CalculateAttachmentFields(WeaponAttachment* attachment)
{
    // attachmentPoint
    if (static_cast<unsigned>(attachment->attachmentType) < ATTACHMENT_TYPE_COUNT)
    {
        attachment->attachmentPoint = attachmentPointByAttachmentTable[attachment->attachmentType];
    }
}

bool AssetLoaderWeaponAttachment::LoadFromInfoString(
    const InfoString& infoString, const std::string& assetName, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone)
{
    auto* attachment = memory->Create<WeaponAttachment>();
    memset(attachment, 0, sizeof(WeaponAttachment));

    InfoStringToWeaponAttachmentConverter converter(
        infoString, attachment, zone->m_script_strings, memory, manager, attachment_fields, std::extent_v<decltype(attachment_fields)>);
    if (!converter.Convert())
    {
        std::cerr << "Failed to parse attachment: \"" << assetName << "\"\n";
        return true;
    }

    CalculateAttachmentFields(attachment);
    attachment->szInternalName = memory->Dup(assetName.c_str());

    manager->AddAsset<AssetAttachment>(
        assetName, attachment, converter.GetDependencies(), converter.GetUsedScriptStrings(), converter.GetIndirectAssetReferences());

    return true;
}

void* AssetLoaderWeaponAttachment::CreateEmptyAsset(const std::string& assetName, MemoryManager* memory)
{
    auto* attachment = memory->Create<WeaponAttachment>();
    memset(attachment, 0, sizeof(WeaponAttachment));
    CalculateAttachmentFields(attachment);
    attachment->szInternalName = memory->Dup(assetName.c_str());
    return attachment;
}

bool AssetLoaderWeaponAttachment::CanLoadFromGdt() const
{
    return true;
}

bool AssetLoaderWeaponAttachment::LoadFromGdt(
    const std::string& assetName, IGdtQueryable* gdtQueryable, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const
{
    const auto* gdtEntry = gdtQueryable->GetGdtEntryByGdfAndName(ObjConstants::GDF_FILENAME_WEAPON_ATTACHMENT, assetName);
    if (gdtEntry == nullptr)
        return false;

    InfoString infoString;
    if (!infoString.FromGdtProperties(*gdtEntry))
    {
        std::cerr << "Failed to read attachment gdt entry: \"" << assetName << "\"\n";
        return true;
    }

    return LoadFromInfoString(infoString, assetName, memory, manager, zone);
}

bool AssetLoaderWeaponAttachment::CanLoadFromRaw() const
{
    return true;
}

bool AssetLoaderWeaponAttachment::LoadFromRaw(
    const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const
{
    const auto fileName = "attachment/" + assetName;
    const auto file = searchPath->Open(fileName);
    if (!file.IsOpen())
        return false;

    InfoString infoString;
    if (!infoString.FromStream(ObjConstants::INFO_STRING_PREFIX_WEAPON_ATTACHMENT, *file.m_stream))
    {
        std::cerr << "Could not parse as info string file: \"" << fileName << "\"\n";
        return true;
    }

    return LoadFromInfoString(infoString, assetName, memory, manager, zone);
}
