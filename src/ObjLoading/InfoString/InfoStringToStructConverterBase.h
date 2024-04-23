#pragma once

#include "InfoString/InfoString.h"
#include "Pool/XAssetInfo.h"
#include "Utils/ClassUtils.h"
#include "Utils/MemoryManager.h"
#include "Zone/ZoneScriptStrings.h"

#include <array>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>

class InfoStringToStructConverterBase
{
protected:
    const InfoString& m_info_string;
    ZoneScriptStrings& m_zone_script_strings;
    std::unordered_set<scr_string_t> m_used_script_string_list;
    std::unordered_set<XAssetInfoGeneric*> m_dependencies;
    std::unordered_set<IndirectAssetReference> m_indirect_asset_references;
    MemoryManager* m_memory;
    void* m_structure;

    static bool ParseAsArray(const std::string& value, std::vector<std::string>& valueArray);

    template<size_t ARRAY_SIZE> static bool ParseAsArray(const std::string& value, std::vector<std::array<std::string, ARRAY_SIZE>>& valueArray)
    {
        static_assert(ARRAY_SIZE >= 1);

        std::array<std::string, ARRAY_SIZE> currentEntry;
        auto currentEntryOffset = 0u;

        for (auto ci = 0u; ci < value.size(); ci++)
        {
            auto c = value[ci];

            if (c == '\r' && ci + 1 < value.size() && value[ci + 1] == '\n')
                c = value[++ci];

            if (c == '\n' && currentEntryOffset != ARRAY_SIZE)
            {
                std::cerr << "Expected value but got new line\n";
                return false;
            }

            if (isspace(c))
                continue;

            int separator;
            const auto startPos = ci;
            while (true)
            {
                ci++;

                // If value ends we use EOF as separator
                if (ci >= value.size())
                {
                    separator = EOF;
                    break;
                }

                c = value[ci];

                // Skip \r from \r\n
                if (c == '\r' && ci + 1 < value.size() && value[ci + 1] == '\n')
                    c = value[++ci];

                // Newline is considered space
                if (isspace(c))
                {
                    separator = static_cast<int>(static_cast<unsigned char>(c));
                    break;
                }
            }

            const auto isNextEntrySeparator = separator == '\n' || separator == EOF;
            const auto isLastEntry = currentEntryOffset >= (ARRAY_SIZE - 1);
            if (isNextEntrySeparator != isLastEntry)
            {
                std::cout << "Expected " << ARRAY_SIZE << " values but got new line\n";
                return false;
            }

            currentEntry[currentEntryOffset++] = std::string(value, startPos, ci - startPos);

            if (isLastEntry)
            {
                valueArray.emplace_back(std::move(currentEntry));
                currentEntryOffset = 0u;
            }
        }

        if (currentEntryOffset > 0)
        {
            std::cout << "Expected " << ARRAY_SIZE << " values but got new line\n";
            return false;
        }

        return true;
    }

    bool ConvertString(const std::string& value, size_t offset);
    bool ConvertStringBuffer(const std::string& value, size_t offset, size_t bufferSize);
    bool ConvertInt(const std::string& value, size_t offset);
    bool ConvertUint(const std::string& value, size_t offset);
    bool ConvertBool(const std::string& value, size_t offset);
    bool ConvertQBoolean(const std::string& value, size_t offset);
    bool ConvertFloat(const std::string& value, size_t offset);
    bool ConvertMilliseconds(const std::string& value, size_t offset);
    bool ConvertScriptString(const std::string& value, size_t offset);
    bool ConvertEnumInt(const std::string& value, size_t offset, const char** enumValues, size_t enumSize);

public:
    InfoStringToStructConverterBase(const InfoString& infoString, void* structure, ZoneScriptStrings& zoneScriptStrings, MemoryManager* memory);
    virtual ~InfoStringToStructConverterBase() = default;
    InfoStringToStructConverterBase(const InfoStringToStructConverterBase& other) = delete;
    InfoStringToStructConverterBase(InfoStringToStructConverterBase&& other) noexcept = delete;
    InfoStringToStructConverterBase& operator=(const InfoStringToStructConverterBase& other) = delete;
    InfoStringToStructConverterBase& operator=(InfoStringToStructConverterBase&& other) noexcept = delete;

    virtual bool Convert() = 0;
    _NODISCARD std::vector<scr_string_t> GetUsedScriptStrings() const;
    _NODISCARD std::vector<XAssetInfoGeneric*> GetDependencies() const;
    _NODISCARD std::vector<IndirectAssetReference> GetIndirectAssetReferences() const;
};
