#include "summoner.hpp"

namespace LibLolAnalyzer
{

    Summoner::Summoner(utility::string_t id, utility::string_t name, uint8_t summoner_level, utility::string_t puuid)
    : m_id(id)
    , m_name(std::move(name))
    , m_summoner_level(summoner_level)
    , m_puuid(std::move(puuid))
    {
    }

    const utility::string_t& Summoner::id() const
    {
        return m_id;
    }

    const utility::string_t& Summoner::name() const
    {
        return m_name;
    }

    uint8_t Summoner::summonerLevel() const
    {
        return m_summoner_level;
    }

    const utility::string_t& Summoner::puuid() const
    {
        return m_puuid;
    }

}
