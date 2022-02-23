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
        struct Participant
        {
            Participant() noexcept = default;
            Participant(const std::size_t kills_, const std::size_t assists_, const std::size_t deaths_,
                        utility::string_t lane_, utility::string_t summoner_id_, std::size_t champion_id_,
                        utility::string_t champion_name_, utility::string_t team_position_,
                        const std::size_t gold_earned_, const std::size_t total_damage_dealt_,
                        const std::size_t total_damage_dealt_to_champions_, const std::size_t total_damage_taken_,
                        const std::size_t total_minions_killed_, const std::size_t vision_score_,
                        const std::size_t vision_wards_bought_in_game_, const std::size_t wards_killed_,
                        const std::size_t wards_placed_);

            std::size_t kills;
            std::size_t assists;
            std::size_t deaths;
            utility::string_t lane;
            utility::string_t summoner_id;
            std::size_t champion_id;
            utility::string_t champion_name;
            utility::string_t team_position;
            std::size_t gold_earned;
            std::size_t total_damage_dealt;
            std::size_t total_damage_dealt_to_champions;
            std::size_t total_damage_taken;
            std::size_t total_minions_killed;
            std::size_t vision_score;
            std::size_t vision_wards_bought_in_game;
            std::size_t wards_killed;
            std::size_t wards_placed;
        };

        Match() noexcept = default;
        Match(const std::size_t game_creation_, const std::size_t game_duration_, const std::size_t game_endTimestamp_,
              const std::size_t game_id_, std::vector<LolApi::Match::Participant> participants_);

        std::size_t game_creation;
        std::size_t game_duration;
        std::size_t game_endTimestamp;
        std::size_t game_id;
        std::vector<LolApi::Match::Participant> participants;
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
