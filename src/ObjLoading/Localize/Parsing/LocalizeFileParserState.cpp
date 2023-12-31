#include "LocalizeFileParserState.h"

#include "Localize/LocalizeCommon.h"

LocalizeFileParserState::LocalizeFileParserState(const GameLanguage language, LocalizeReadingZoneState* zoneState)
    : m_end(false),
      m_language(language),
      m_zone_state(zoneState)
{
    m_language_name_caps = LocalizeCommon::GetNameOfLanguage(m_language);
    for (auto& c : m_language_name_caps)
        c = static_cast<char>(toupper(c));
}
