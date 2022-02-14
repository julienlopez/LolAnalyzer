#pragma once

#include "ExpectedTask.hpp"

#include "summoner.hpp"

#include <utility>

#include <boost/optional.hpp>

#include <cpprest/json.h>

namespace LibLolAnalyzer
{

class LolApi
{
public:
    struct MatchListOptions
    {
        boost::optional<utility::string_t> type;
        boost::optional<std::size_t> start;
        boost::optional<std::size_t> count;
        boost::optional<std::size_t> start_time;
        boost::optional<std::size_t> end_time;
        boost::optional<std::size_t> queue;
    };

    struct Match
    {
        Match() noexcept = default;
        Match(const std::size_t game_creation_, const std::size_t game_duration_, const std::size_t game_endTimestamp_,
              const std::size_t game_id_) noexcept;

        std::size_t game_creation;
        std::size_t game_duration;
        std::size_t game_endTimestamp;
        std::size_t game_id;
    };

    LolApi(utility::string_t api_key);

    LolApi(const LolApi&) = delete;
    LolApi(LolApi&&) = delete;

    LolApi& operator=(const LolApi&) = delete;
    LolApi& operator=(LolApi&&) = delete;

    ~LolApi() = default;

    LibRest::ExpectedTask<Summoner> requestSummonerByName(const utility::string_t& region,
                                                          const utility::string_t& summoner_name) const;

    LibRest::ExpectedTask<std::vector<utility::string_t>> requestMatchlist(const utility::string_t& region,
                                                                           const utility::string_t& summoner_id,
                                                                           const MatchListOptions& options = {}) const;

    LibRest::ExpectedTask<Match> requestMatch(const utility::string_t& region, const utility::string_t& match_id) const;

private:
    const utility::string_t m_api_key;
};

} // namespace LibLolAnalyzer
