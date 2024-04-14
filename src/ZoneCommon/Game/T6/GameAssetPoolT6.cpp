#include "GameAssetPoolT6.h"

#include "Pool/AssetPoolDynamic.h"
#include "Pool/AssetPoolStatic.h"

#include <cassert>
#include <type_traits>

using namespace T6;

const char* GameAssetPoolT6::ASSET_TYPE_NAMES[]{
    "xmodelpieces",
    "physpreset",
    "physconstraints",
    "destructibledef",
    "xanim",
    "xmodel",
    "material",
    "techniqueset",
    "image",
    "soundbank",
    "soundpatch",
    "clipmap",
    "clipmap",
    "comworld",
    "gameworldsp",
    "gameworldmp",
    "mapents",
    "gfxworld",
    "gfxlightdef",
    "uimap",
    "font",
    "fonticon",
    "menulist",
    "menu",
    "localize",
    "weapon",
    "weapondef",
    "weaponvariant",
    "weaponfull",
    "attachment",
    "attachmentunique",
    "camo",
    "snddriverglobals",
    "fx",
    "fximpacttable",
    "aitype",
    "mptype",
    "mpbody",
    "mphead",
    "character",
    "xmodelalias",
    "rawfile",
    "stringtable",
    "leaderboard",
    "xglobals",
    "ddl",
    "glasses",
    "emblemset",
    "script",
    "keyvaluepairs",
    "vehicle",
    "memoryblock",
    "addonmapents",
    "tracer",
    "skinnedverts",
    "qdb",
    "slug",
    "footsteptable",
    "footstepfxtable",
    "zbarrier",
};

GameAssetPoolT6::GameAssetPoolT6(Zone* zone, const int priority)
    : ZoneAssetPools(zone),
      m_priority(priority)
{
    assert(std::extent_v<decltype(ASSET_TYPE_NAMES)> == ASSET_TYPE_COUNT);
}

void GameAssetPoolT6::InitPoolStatic(const asset_type_t type, const size_t capacity)
{
#define CASE_INIT_POOL_STATIC(assetType, poolName)                                                                                                             \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        if ((poolName) == nullptr && capacity > 0)                                                                                                             \
        {                                                                                                                                                      \
            (poolName) = std::make_unique<AssetPoolStatic<decltype(poolName)::element_type::type>>(capacity, m_priority, (assetType));                         \
        }                                                                                                                                                      \
        break;                                                                                                                                                 \
    }

    switch (type)
    {
        CASE_INIT_POOL_STATIC(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_PHYSCONSTRAINTS, m_phys_constraints)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_DESTRUCTIBLEDEF, m_destructible_def)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_MATERIAL, m_material)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_IMAGE, m_image)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SOUND, m_sound_bank)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SOUND_PATCH, m_sound_patch)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_FONT, m_font)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_FONTICON, m_font_icon)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_MENU, m_menu_def)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_WEAPON, m_weapon)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_ATTACHMENT, m_attachment)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_ATTACHMENT_UNIQUE, m_attachment_unique)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_WEAPON_CAMO, m_camo)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SNDDRIVER_GLOBALS, m_snd_driver_globals)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_FX, m_fx)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_STRINGTABLE, m_string_table)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_LEADERBOARD, m_leaderboard)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_XGLOBALS, m_xglobals)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_DDL, m_ddl)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_GLASSES, m_glasses)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_EMBLEMSET, m_emblem_set)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SCRIPTPARSETREE, m_script)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_KEYVALUEPAIRS, m_key_value_pairs)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_VEHICLEDEF, m_vehicle)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_MEMORYBLOCK, m_memory_block)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_ADDON_MAP_ENTS, m_addon_map_ents)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_TRACER, m_tracer)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SKINNEDVERTS, m_skinned_verts)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_QDB, m_qdb)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_SLUG, m_slug)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_FOOTSTEP_TABLE, m_footstep_table)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_FOOTSTEPFX_TABLE, m_footstep_fx_table)
        CASE_INIT_POOL_STATIC(ASSET_TYPE_ZBARRIER, m_zbarrier)

    default:
        assert(type >= 0 && type < ASSET_TYPE_COUNT);
        break;
    }

#undef CASE_INIT_POOL_STATIC
}

void GameAssetPoolT6::InitPoolDynamic(const asset_type_t type)
{
#define CASE_INIT_POOL_DYNAMIC(assetType, poolName)                                                                                                            \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        if ((poolName) == nullptr)                                                                                                                             \
        {                                                                                                                                                      \
            (poolName) = std::make_unique<AssetPoolDynamic<decltype(poolName)::element_type::type>>(m_priority, (assetType));                                  \
        }                                                                                                                                                      \
        break;                                                                                                                                                 \
    }

    switch (type)
    {
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_PHYSCONSTRAINTS, m_phys_constraints)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_DESTRUCTIBLEDEF, m_destructible_def)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_MATERIAL, m_material)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_IMAGE, m_image)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SOUND, m_sound_bank)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SOUND_PATCH, m_sound_patch)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_FONT, m_font)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_FONTICON, m_font_icon)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_MENU, m_menu_def)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_WEAPON, m_weapon)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_ATTACHMENT, m_attachment)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_ATTACHMENT_UNIQUE, m_attachment_unique)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_WEAPON_CAMO, m_camo)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SNDDRIVER_GLOBALS, m_snd_driver_globals)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_FX, m_fx)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_STRINGTABLE, m_string_table)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_LEADERBOARD, m_leaderboard)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_XGLOBALS, m_xglobals)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_DDL, m_ddl)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_GLASSES, m_glasses)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_EMBLEMSET, m_emblem_set)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SCRIPTPARSETREE, m_script)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_KEYVALUEPAIRS, m_key_value_pairs)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_VEHICLEDEF, m_vehicle)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_MEMORYBLOCK, m_memory_block)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_ADDON_MAP_ENTS, m_addon_map_ents)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_TRACER, m_tracer)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SKINNEDVERTS, m_skinned_verts)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_QDB, m_qdb)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_SLUG, m_slug)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_FOOTSTEP_TABLE, m_footstep_table)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_FOOTSTEPFX_TABLE, m_footstep_fx_table)
        CASE_INIT_POOL_DYNAMIC(ASSET_TYPE_ZBARRIER, m_zbarrier)

    default:
        assert(type >= 0 && type < ASSET_TYPE_COUNT);
        break;
    }

#undef CASE_INIT_POOL_DYNAMIC
}

XAssetInfoGeneric* GameAssetPoolT6::AddAssetToPool(std::unique_ptr<XAssetInfoGeneric> xAssetInfo)
{
#define CASE_ADD_TO_POOL(assetType, poolName)                                                                                                                  \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        assert((poolName) != nullptr);                                                                                                                         \
        return (poolName)->AddAsset(std::unique_ptr<XAssetInfo<decltype(poolName)::element_type::type>>(                                                       \
            static_cast<XAssetInfo<decltype(poolName)::element_type::type>*>(xAssetInfo.release())));                                                          \
    }

    switch (static_cast<XAssetType>(xAssetInfo->m_type))
    {
        CASE_ADD_TO_POOL(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_ADD_TO_POOL(ASSET_TYPE_PHYSCONSTRAINTS, m_phys_constraints)
        CASE_ADD_TO_POOL(ASSET_TYPE_DESTRUCTIBLEDEF, m_destructible_def)
        CASE_ADD_TO_POOL(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_ADD_TO_POOL(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_ADD_TO_POOL(ASSET_TYPE_MATERIAL, m_material)
        CASE_ADD_TO_POOL(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_ADD_TO_POOL(ASSET_TYPE_IMAGE, m_image)
        CASE_ADD_TO_POOL(ASSET_TYPE_SOUND, m_sound_bank)
        CASE_ADD_TO_POOL(ASSET_TYPE_SOUND_PATCH, m_sound_patch)
        CASE_ADD_TO_POOL(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_ADD_TO_POOL(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_ADD_TO_POOL(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_ADD_TO_POOL(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_ADD_TO_POOL(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_ADD_TO_POOL(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_ADD_TO_POOL(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_ADD_TO_POOL(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_ADD_TO_POOL(ASSET_TYPE_FONT, m_font)
        CASE_ADD_TO_POOL(ASSET_TYPE_FONTICON, m_font_icon)
        CASE_ADD_TO_POOL(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_ADD_TO_POOL(ASSET_TYPE_MENU, m_menu_def)
        CASE_ADD_TO_POOL(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_ADD_TO_POOL(ASSET_TYPE_WEAPON, m_weapon)
        CASE_ADD_TO_POOL(ASSET_TYPE_ATTACHMENT, m_attachment)
        CASE_ADD_TO_POOL(ASSET_TYPE_ATTACHMENT_UNIQUE, m_attachment_unique)
        CASE_ADD_TO_POOL(ASSET_TYPE_WEAPON_CAMO, m_camo)
        CASE_ADD_TO_POOL(ASSET_TYPE_SNDDRIVER_GLOBALS, m_snd_driver_globals)
        CASE_ADD_TO_POOL(ASSET_TYPE_FX, m_fx)
        CASE_ADD_TO_POOL(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_ADD_TO_POOL(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_ADD_TO_POOL(ASSET_TYPE_STRINGTABLE, m_string_table)
        CASE_ADD_TO_POOL(ASSET_TYPE_LEADERBOARD, m_leaderboard)
        CASE_ADD_TO_POOL(ASSET_TYPE_XGLOBALS, m_xglobals)
        CASE_ADD_TO_POOL(ASSET_TYPE_DDL, m_ddl)
        CASE_ADD_TO_POOL(ASSET_TYPE_GLASSES, m_glasses)
        CASE_ADD_TO_POOL(ASSET_TYPE_EMBLEMSET, m_emblem_set)
        CASE_ADD_TO_POOL(ASSET_TYPE_SCRIPTPARSETREE, m_script)
        CASE_ADD_TO_POOL(ASSET_TYPE_KEYVALUEPAIRS, m_key_value_pairs)
        CASE_ADD_TO_POOL(ASSET_TYPE_VEHICLEDEF, m_vehicle)
        CASE_ADD_TO_POOL(ASSET_TYPE_MEMORYBLOCK, m_memory_block)
        CASE_ADD_TO_POOL(ASSET_TYPE_ADDON_MAP_ENTS, m_addon_map_ents)
        CASE_ADD_TO_POOL(ASSET_TYPE_TRACER, m_tracer)
        CASE_ADD_TO_POOL(ASSET_TYPE_SKINNEDVERTS, m_skinned_verts)
        CASE_ADD_TO_POOL(ASSET_TYPE_QDB, m_qdb)
        CASE_ADD_TO_POOL(ASSET_TYPE_SLUG, m_slug)
        CASE_ADD_TO_POOL(ASSET_TYPE_FOOTSTEP_TABLE, m_footstep_table)
        CASE_ADD_TO_POOL(ASSET_TYPE_FOOTSTEPFX_TABLE, m_footstep_fx_table)
        CASE_ADD_TO_POOL(ASSET_TYPE_ZBARRIER, m_zbarrier)

    default:
        assert(false);
        break;
    }

    return nullptr;

#undef CASE_ADD_TO_POOL
}

XAssetInfoGeneric* GameAssetPoolT6::GetAsset(const asset_type_t type, const std::string& name) const
{
#define CASE_GET_ASSET(assetType, poolName)                                                                                                                    \
    case assetType:                                                                                                                                            \
    {                                                                                                                                                          \
        if ((poolName) != nullptr)                                                                                                                             \
            return (poolName)->GetAsset(name);                                                                                                                 \
        break;                                                                                                                                                 \
    }

    switch (type)
    {
        CASE_GET_ASSET(ASSET_TYPE_PHYSPRESET, m_phys_preset)
        CASE_GET_ASSET(ASSET_TYPE_PHYSCONSTRAINTS, m_phys_constraints)
        CASE_GET_ASSET(ASSET_TYPE_DESTRUCTIBLEDEF, m_destructible_def)
        CASE_GET_ASSET(ASSET_TYPE_XANIMPARTS, m_xanim_parts)
        CASE_GET_ASSET(ASSET_TYPE_XMODEL, m_xmodel)
        CASE_GET_ASSET(ASSET_TYPE_MATERIAL, m_material)
        CASE_GET_ASSET(ASSET_TYPE_TECHNIQUE_SET, m_technique_set)
        CASE_GET_ASSET(ASSET_TYPE_IMAGE, m_image)
        CASE_GET_ASSET(ASSET_TYPE_SOUND, m_sound_bank)
        CASE_GET_ASSET(ASSET_TYPE_SOUND_PATCH, m_sound_patch)
        CASE_GET_ASSET(ASSET_TYPE_CLIPMAP, m_clip_map)
        CASE_GET_ASSET(ASSET_TYPE_CLIPMAP_PVS, m_clip_map)
        CASE_GET_ASSET(ASSET_TYPE_COMWORLD, m_com_world)
        CASE_GET_ASSET(ASSET_TYPE_GAMEWORLD_SP, m_game_world_sp)
        CASE_GET_ASSET(ASSET_TYPE_GAMEWORLD_MP, m_game_world_mp)
        CASE_GET_ASSET(ASSET_TYPE_MAP_ENTS, m_map_ents)
        CASE_GET_ASSET(ASSET_TYPE_GFXWORLD, m_gfx_world)
        CASE_GET_ASSET(ASSET_TYPE_LIGHT_DEF, m_gfx_light_def)
        CASE_GET_ASSET(ASSET_TYPE_FONT, m_font)
        CASE_GET_ASSET(ASSET_TYPE_FONTICON, m_font_icon)
        CASE_GET_ASSET(ASSET_TYPE_MENULIST, m_menu_list)
        CASE_GET_ASSET(ASSET_TYPE_MENU, m_menu_def)
        CASE_GET_ASSET(ASSET_TYPE_LOCALIZE_ENTRY, m_localize)
        CASE_GET_ASSET(ASSET_TYPE_WEAPON, m_weapon)
        CASE_GET_ASSET(ASSET_TYPE_ATTACHMENT, m_attachment)
        CASE_GET_ASSET(ASSET_TYPE_ATTACHMENT_UNIQUE, m_attachment_unique)
        CASE_GET_ASSET(ASSET_TYPE_WEAPON_CAMO, m_camo)
        CASE_GET_ASSET(ASSET_TYPE_SNDDRIVER_GLOBALS, m_snd_driver_globals)
        CASE_GET_ASSET(ASSET_TYPE_FX, m_fx)
        CASE_GET_ASSET(ASSET_TYPE_IMPACT_FX, m_fx_impact_table)
        CASE_GET_ASSET(ASSET_TYPE_RAWFILE, m_raw_file)
        CASE_GET_ASSET(ASSET_TYPE_STRINGTABLE, m_string_table)
        CASE_GET_ASSET(ASSET_TYPE_LEADERBOARD, m_leaderboard)
        CASE_GET_ASSET(ASSET_TYPE_XGLOBALS, m_xglobals)
        CASE_GET_ASSET(ASSET_TYPE_DDL, m_ddl)
        CASE_GET_ASSET(ASSET_TYPE_GLASSES, m_glasses)
        CASE_GET_ASSET(ASSET_TYPE_EMBLEMSET, m_emblem_set)
        CASE_GET_ASSET(ASSET_TYPE_SCRIPTPARSETREE, m_script)
        CASE_GET_ASSET(ASSET_TYPE_KEYVALUEPAIRS, m_key_value_pairs)
        CASE_GET_ASSET(ASSET_TYPE_VEHICLEDEF, m_vehicle)
        CASE_GET_ASSET(ASSET_TYPE_MEMORYBLOCK, m_memory_block)
        CASE_GET_ASSET(ASSET_TYPE_ADDON_MAP_ENTS, m_addon_map_ents)
        CASE_GET_ASSET(ASSET_TYPE_TRACER, m_tracer)
        CASE_GET_ASSET(ASSET_TYPE_SKINNEDVERTS, m_skinned_verts)
        CASE_GET_ASSET(ASSET_TYPE_QDB, m_qdb)
        CASE_GET_ASSET(ASSET_TYPE_SLUG, m_slug)
        CASE_GET_ASSET(ASSET_TYPE_FOOTSTEP_TABLE, m_footstep_table)
        CASE_GET_ASSET(ASSET_TYPE_FOOTSTEPFX_TABLE, m_footstep_fx_table)
        CASE_GET_ASSET(ASSET_TYPE_ZBARRIER, m_zbarrier)

    default:
        assert(false);
        break;
    }

    return nullptr;

#undef CASE_GET_ASSET
}

const char* GameAssetPoolT6::AssetTypeNameByType(const asset_type_t assetType)
{
    if (assetType >= 0 && assetType < static_cast<int>(std::extent_v<decltype(ASSET_TYPE_NAMES)>))
        return ASSET_TYPE_NAMES[assetType];

    return ASSET_TYPE_INVALID;
}

const char* GameAssetPoolT6::GetAssetTypeName(const asset_type_t assetType) const
{
    return AssetTypeNameByType(assetType);
}

asset_type_t GameAssetPoolT6::AssetTypeCount()
{
    return ASSET_TYPE_COUNT;
}

asset_type_t GameAssetPoolT6::GetAssetTypeCount() const
{
    return AssetTypeCount();
}
