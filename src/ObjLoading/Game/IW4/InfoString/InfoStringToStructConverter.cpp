#include "InfoStringToStructConverter.h"

#include <cassert>
#include <iostream>

using namespace IW4;

InfoStringToStructConverter::InfoStringToStructConverter(const InfoString& infoString,
                                                         void* structure,
                                                         ZoneScriptStrings& zoneScriptStrings,
                                                         MemoryManager* memory,
                                                         IAssetLoadingManager* manager,
                                                         const cspField_t* fields,
                                                         const size_t fieldCount)
    : InfoStringToStructConverterBase(infoString, structure, zoneScriptStrings, memory),
      m_loading_manager(manager),
      m_fields(fields),
      m_field_count(fieldCount)
{
}

bool InfoStringToStructConverter::ConvertBaseField(const cspField_t& field, const std::string& value)
{
    switch (static_cast<csParseFieldType_t>(field.iFieldType))
    {
    case CSPFT_STRING:
        return ConvertString(value, field.iOffset);

    case CSPFT_STRING_MAX_STRING_CHARS:
        return ConvertStringBuffer(value, field.iOffset, 1024);

    case CSPFT_STRING_MAX_QPATH:
        return ConvertStringBuffer(value, field.iOffset, 64);

    case CSPFT_STRING_MAX_OSPATH:
        return ConvertStringBuffer(value, field.iOffset, 256);

    case CSPFT_INT:
        return ConvertInt(value, field.iOffset);

    case CSPFT_BOOL:
        return ConvertBool(value, field.iOffset);

    case CSPFT_QBOOLEAN:
        return ConvertQBoolean(value, field.iOffset);

    case CSPFT_FLOAT:
        return ConvertFloat(value, field.iOffset);

    case CSPFT_MILLISECONDS:
        return ConvertMilliseconds(value, field.iOffset);

    case CSPFT_FX:
    {
        if (value.empty())
        {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = nullptr;
            return true;
        }

        auto* fx = m_loading_manager->LoadDependency(ASSET_TYPE_FX, value);

        if (fx == nullptr)
        {
            std::cout << "Failed to load fx asset \"" << value << "\"\n";
            return false;
        }

        m_dependencies.emplace(fx);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = fx->m_ptr;

        return true;
    }

    case CSPFT_XMODEL:
    {
        if (value.empty())
        {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = nullptr;
            return true;
        }

        auto* xmodel = m_loading_manager->LoadDependency(ASSET_TYPE_XMODEL, value);

        if (xmodel == nullptr)
        {
            std::cout << "Failed to load xmodel asset \"" << value << "\"\n";
            return false;
        }

        m_dependencies.emplace(xmodel);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = xmodel->m_ptr;

        return true;
    }

    case CSPFT_MATERIAL:
    {
        if (value.empty())
        {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = nullptr;
            return true;
        }

        auto* material = m_loading_manager->LoadDependency(ASSET_TYPE_MATERIAL, value);

        if (material == nullptr)
        {
            std::cout << "Failed to load material asset \"" << value << "\"\n";
            return false;
        }

        m_dependencies.emplace(material);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = material->m_ptr;

        return true;
    }

    case CSPFT_TRACER:
    {
        if (value.empty())
        {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = nullptr;
            return true;
        }

        auto* tracer = m_loading_manager->LoadDependency(ASSET_TYPE_TRACER, value);

        if (tracer == nullptr)
        {
            std::cout << "Failed to load tracer asset \"" << value << "\"\n";
            return false;
        }

        m_dependencies.emplace(tracer);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = tracer->m_ptr;

        return true;
    }

    case CSPFT_MPH_TO_INCHES_PER_SEC:
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

    case CSPFT_PHYS_COLLMAP:
    {
        if (value.empty())
        {
            *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = nullptr;
            return true;
        }

        auto* collmap = m_loading_manager->LoadDependency(ASSET_TYPE_PHYSCOLLMAP, value);

        if (collmap == nullptr)
        {
            std::cout << "Failed to load collmap asset \"" << value << "\"\n";
            return false;
        }

        m_dependencies.emplace(collmap);
        *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset) = collmap->m_ptr;

        return true;
    }

    case CSPFT_SOUND:
    {
        if (value.empty())
        {
            reinterpret_cast<SndAliasCustom*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset)->name = nullptr;
            return true;
        }

        auto* name = static_cast<snd_alias_list_name*>(m_memory->Alloc(sizeof(snd_alias_list_name)));
        name->soundName = m_memory->Dup(value.c_str());

        reinterpret_cast<SndAliasCustom*>(reinterpret_cast<uintptr_t>(m_structure) + field.iOffset)->name = name;

        m_indirect_asset_references.emplace(ASSET_TYPE_SOUND, value);
        return true;
    }

    case CSPFT_NUM_BASE_FIELD_TYPES:
    default:
        assert(false);
        return false;
    }
}

bool InfoStringToStructConverter::Convert()
{
    for (auto fieldIndex = 0u; fieldIndex < m_field_count; fieldIndex++)
    {
        const auto& field = m_fields[fieldIndex];
        assert(field.iFieldType >= 0);

        auto foundValue = false;
        const auto& value = m_info_string.GetValueForKey(std::string(field.szName), &foundValue);

        if (foundValue)
        {
            if (field.iFieldType < CSPFT_NUM_BASE_FIELD_TYPES)
            {
                if (!ConvertBaseField(field, value))
                    return false;
            }
            else
            {
                if (!ConvertExtensionField(field, value))
                    return false;
            }
        }
    }

    return true;
}
