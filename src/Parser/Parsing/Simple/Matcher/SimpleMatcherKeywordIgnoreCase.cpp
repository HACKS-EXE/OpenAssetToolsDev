#include "SimpleMatcherKeywordIgnoreCase.h"

#include <algorithm>

SimpleMatcherKeywordIgnoreCase::SimpleMatcherKeywordIgnoreCase(std::string value)
    : m_value(std::move(value))
{
    for (auto& c : m_value)
        c = static_cast<char>(tolower(c));
}

MatcherResult<SimpleParserValue> SimpleMatcherKeywordIgnoreCase::CanMatch(ILexer<SimpleParserValue>* lexer, const unsigned tokenOffset)
{
    const auto& token = lexer->GetToken(tokenOffset);

    if (token.m_type != SimpleParserValueType::IDENTIFIER)
        return MatcherResult<SimpleParserValue>::NoMatch();

    const auto& identifierValue = token.IdentifierValue();
    const auto isEqual = std::ranges::equal(identifierValue,
                                            m_value,
                                            [](const char a, const char b)
                                            {
                                                return tolower(a) == b;
                                            });

    if (isEqual)
        return MatcherResult<SimpleParserValue>::Match(1);

    return MatcherResult<SimpleParserValue>::NoMatch();
}
