#include "AssetLoaderVehicle.h"

#include "Game/T6/InfoString/EnumStrings.h"
#include "Game/T6/InfoString/InfoStringToStructConverter.h"
#include "Game/T6/InfoString/VehicleFields.h"
#include "Game/T6/ObjConstantsT6.h"
#include "Game/T6/T6.h"
#include "InfoString/InfoString.h"
#include "Pool/GlobalAssetPool.h"

#include <cstring>
#include <iostream>

using namespace T6;

namespace T6
{
    class InfoStringToVehicleConverter final : public InfoStringToStructConverter
    {
    protected:
        bool ConvertExtensionField(const cspField_t& field, const std::string& value) override
        {
            switch (static_cast<VehicleFieldType>(field.iFieldType))
            {
            case VFT_TYPE:
                return ConvertEnumInt(field.szName, value, field.iOffset, s_vehicleClassNames, std::extent_v<decltype(s_vehicleClassNames)>);

            case VFT_CAMERAMODE:
                return ConvertEnumInt(field.szName, value, field.iOffset, s_vehicleCameraModes, std::extent_v<decltype(s_vehicleCameraModes)>);

            case VFT_TRACTION_TYPE:
                return ConvertEnumInt(field.szName, value, field.iOffset, s_tractionTypeNames, std::extent_v<decltype(s_tractionTypeNames)>);

            case VFT_MPH_TO_INCHES_PER_SECOND:
            {
                char* endPtr;
                *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = strtof(value.c_str(), &endPtr) * 17.6f;

                if (endPtr != &value[value.size()])
                {
                    std::cout << "Failed to parse value \"" << value << "\" as mph\n";
                    return false;
                }

                return true;
            }

            case VFT_POUNDS_TO_GAME_MASS:
            {
                char* endPtr;
                *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = strtof(value.c_str(), &endPtr) * 0.001f;

                if (endPtr != &value[value.size()])
                {
                    std::cout << "Failed to parse value \"" << value << "\" as pounds\n";
                    return false;
                }

                return true;
            }

            case VFT_TEAM:
            {
                if (value == "axis")
                {
                    *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = TEAM_AXIS;
                    return true;
                }

                if (value == "allies")
                {
                    *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = TEAM_ALLIES;
                    return true;
                }

                if (value == "neutral")
                {
                    *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = TEAM_FOUR;
                    return true;
                }

                *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = TEAM_BAD;
                std::cout << "Failed to parse value \"" << value << "\" as team\n";
                return false;
            }

            case VFT_KEY_BINDING:
            case VFT_GRAPH:
            case VFT_WIIUCONTROLOVERRIDE:
            case VFT_NUM:
            default:
                assert(false);
                return false;
            }
        }

    public:
        InfoStringToVehicleConverter(const InfoString& infoString,
                                     VehicleDef* vehicleDef,
                                     ZoneScriptStrings& zoneScriptStrings,
                                     MemoryManager* memory,
                                     IAssetLoadingManager* manager,
                                     const cspField_t* fields,
                                     const size_t fieldCount)
            : InfoStringToStructConverter(infoString, vehicleDef, zoneScriptStrings, memory, manager, fields, fieldCount)
        {
        }
    };
} // namespace T6

bool AssetLoaderVehicle::LoadFromInfoString(
    const InfoString& infoString, const std::string& assetName, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone)
{
    auto* vehicleDef = memory->Create<VehicleDef>();
    memset(vehicleDef, 0, sizeof(VehicleDef));

    InfoStringToVehicleConverter converter(
        infoString, vehicleDef, zone->m_script_strings, memory, manager, vehicle_fields, std::extent_v<decltype(vehicle_fields)>);
    if (!converter.Convert())
    {
        std::cout << "Failed to parse vehicle: \"" << assetName << "\"\n";
        return true;
    }

    vehicleDef->name = memory->Dup(assetName.c_str());

    manager->AddAsset<AssetVehicle>(assetName, vehicleDef, converter.GetDependencies(), converter.GetUsedScriptStrings());

    return true;
}

void* AssetLoaderVehicle::CreateEmptyAsset(const std::string& assetName, MemoryManager* memory)
{
    auto* vehicleDef = memory->Create<VehicleDef>();
    memset(vehicleDef, 0, sizeof(VehicleDef));
    vehicleDef->name = memory->Dup(assetName.c_str());
    return vehicleDef;
}

bool AssetLoaderVehicle::CanLoadFromGdt() const
{
    return true;
}

bool AssetLoaderVehicle::LoadFromGdt(
    const std::string& assetName, IGdtQueryable* gdtQueryable, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const
{
    auto* gdtEntry = gdtQueryable->GetGdtEntryByGdfAndName(ObjConstants::GDF_FILENAME_VEHICLE, assetName);
    if (gdtEntry == nullptr)
        return false;

    InfoString infoString;
    if (!infoString.FromGdtProperties(*gdtEntry))
    {
        std::cout << "Failed to read vehicle gdt entry: \"" << assetName << "\"\n";
        return true;
    }

    return LoadFromInfoString(infoString, assetName, memory, manager, zone);
}

bool AssetLoaderVehicle::CanLoadFromRaw() const
{
    return true;
}

bool AssetLoaderVehicle::LoadFromRaw(
    const std::string& assetName, ISearchPath* searchPath, MemoryManager* memory, IAssetLoadingManager* manager, Zone* zone) const
{
    const auto fileName = "vehicles/" + assetName;
    const auto file = searchPath->Open(fileName);
    if (!file.IsOpen())
        return false;

    InfoString infoString;
    if (!infoString.FromStream(ObjConstants::INFO_STRING_PREFIX_VEHICLE, *file.m_stream))
    {
        std::cerr << "Could not parse as info string file: \"" << fileName << "\"\n";
        return true;
    }

    return LoadFromInfoString(infoString, assetName, memory, manager, zone);
}
