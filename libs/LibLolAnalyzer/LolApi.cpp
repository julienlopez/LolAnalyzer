#include "lolapi.hpp"

#include "JsonParser.hpp"
#include "Result.hpp"
#include "Utils.hpp"

#include <boost/hof/construct.hpp>

#include <cpprest/http_client.h>

namespace LibRest::JsonParser
{

template <>
LibRest::Result<LibLolAnalyzer::LolApi::Match::Participant> inline cast<LibLolAnalyzer::LolApi::Match::Participant>(
    const web::json::value& value)
{
    return LibRest::JsonParser::parseSimpleObject<std::size_t, std::size_t, std::size_t, utility::string_t,
                                                  utility::string_t, std::size_t, utility::string_t,
                                                  utility::string_t, std::size_t, std::size_t, std::size_t, std::size_t,
                                                  std::size_t, std::size_t, std::size_t, std::size_t, std::size_t>(
               value, U("kills"), U("assists"), U("deaths"), U("lane"), U("summonerId"), U("championId"),
               U("championName"), U("teamPosition"), U("goldEarned"), U("totalDamageDealt"),
               U("totalDamageDealtToChampions"), U("totalDamageTaken"), U("totalMinionsKilled"), U("visionScore"),
               U("visionWardsBoughtInGame"), U("wardsKilled"), U("wardsPlaced"))
        .map(LibRest::Algorithms::applyToTuple(boost::hof::construct<LibLolAnalyzer::LolApi::Match::Participant>()));
}

template <>
LibRest::Result<std::vector<LibLolAnalyzer::LolApi::Match::Participant>> inline cast<
    std::vector<LibLolAnalyzer::LolApi::Match::Participant>>(const web::json::value& value)
{
    return parseArray<LibLolAnalyzer::LolApi::Match::Participant>(value);
}

} // namespace LibRest::JsonParser

namespace LibLolAnalyzer
{

namespace
{

    LibRest::Result<Summoner> parseSummoner(const web::json::value& value)
    {
        ucout << value << std::endl;
        return LibRest::JsonParser::parseSimpleObject<utility::string_t, utility::string_t, std::size_t,
                                                      utility::string_t>(value, U("id"), U("name"), U("summonerLevel"),
                                                                         U("puuid"))
            .map(LibRest::Algorithms::applyToTuple(boost::hof::construct<Summoner>()));
    }

    LibRest::Result<std::vector<utility::string_t>> parseMatchList(const web::json::value& value)
    {
        return LibRest::JsonParser::parseArray<utility::string_t>(value);
    }

    LibRest::ExpectedTask<web::json::value>
    requestAPI(const utility::string_t& region, const utility::string_t& request, const utility::string_t& api_key,
               const utility::string_t& option = {}, const utility::string_t prefix_region = {},
               const std::map<utility::string_t, utility::string_t>& additional_parameters = {})
    {
        return expected_task::create_task<utility::string_t>(
                   [=]()
                   {
                       auto url = U("https://") + (prefix_region.empty() ? region : prefix_region)
                                  + U(".api.riotgames.com/lol/");
                       if(!option.empty()) url += option + U("/");
                       web::http::client::http_client client(url);
                       web::uri_builder builder(request);
                       builder.append_query(U("api_key"), api_key);
                       for(const auto& p : additional_parameters)
                           builder.append_query(p.first, p.second);
                       ucout << builder.to_string() << std::endl;
                       return client.request(web::http::methods::GET, builder.to_string());
                   })
            .and_then(LibRest::Utils::extractJson());
    }

    std::map<utility::string_t, utility::string_t> toparams(const LolApi::MatchListOptions& options)
    {
        std::map<utility::string_t, utility::string_t> params;
        if(options.type) params[U("type")] = *options.type;
        if(options.start) params[U("start")] = std::to_wstring(*options.start);
        if(options.count) params[U("count")] = std::to_wstring(*options.count);
        if(options.start_time) params[U("startTime")] = std::to_wstring(*options.start_time);
        if(options.end_time) params[U("endTime")] = std::to_wstring(*options.end_time);
        if(options.queue) params[U("queue")] = std::to_wstring(*options.queue);
        return params;
    }

    LibRest::Result<LolApi::Match> parseMatchInfo(const web::json::value& value)
    {
        return LibRest::JsonParser::parseSimpleObject<std::size_t, std::size_t, std::size_t, std::size_t,
                                                      std::vector<LolApi::Match::Participant>>(
                   value, U("gameCreation"), U("gameDuration"), U("gameEndTimestamp"), U("gameId"), U("participants"))
            .map(LibRest::Algorithms::applyToTuple(boost::hof::construct<LolApi::Match>()));
    }

    LibRest::Result<LolApi::Match> parseMatch(const web::json::value& value)
    {
        if(not value.is_object()) return tl::make_unexpected(U("match response is not an object"));
        const auto& obj = value.as_object();
        const auto it = obj.find(U("info"));
        if(it == std::end(obj)) return tl::make_unexpected(U("unable to find the info field in the match response"));
        return parseMatchInfo(it->second);
    }

} // namespace

LolApi::Match::Participant::Participant(const std::size_t kills_, const std::size_t assists_, const std::size_t deaths_,
                                        utility::string_t lane_, utility::string_t summoner_id_,
                                        std::size_t champion_id_, utility::string_t champion_name_,
                                        utility::string_t team_position_, const std::size_t gold_earned_,
                                        const std::size_t total_damage_dealt_,
                                        const std::size_t total_damage_dealt_to_champions_,
                                        const std::size_t total_damage_taken_, const std::size_t total_minions_killed_,
                                        const std::size_t vision_score_, const std::size_t vision_wards_bought_in_game_,
                                        const std::size_t wards_killed_, const std::size_t wards_placed_)
    : kills{kills_}
    , assists{assists_}
    , deaths{deaths_}
    , lane{std::move(lane_)}
    , summoner_id{std::move(summoner_id_)}
    , champion_id{champion_id_}
    , champion_name{std::move(champion_name_)}
    , team_position{std::move(team_position_)}
    , gold_earned{gold_earned_}
    , total_damage_dealt{total_damage_dealt_}
    , total_damage_dealt_to_champions{total_damage_dealt_to_champions_}
    , total_damage_taken{total_damage_taken_}
    , total_minions_killed{total_minions_killed_}
    , vision_score{vision_score_}
    , vision_wards_bought_in_game{vision_wards_bought_in_game_}
    , wards_killed{wards_killed_}
    , wards_placed{wards_placed_}
{
}

LolApi::Match::Match(const std::size_t game_creation_, const std::size_t game_duration_,
                     const std::size_t game_endTimestamp_, const std::size_t game_id_,
                     std::vector<LolApi::Match::Participant> participants_)
    : game_creation(game_creation_)
    , game_duration(game_duration_)
    , game_endTimestamp(game_endTimestamp_)
    , game_id(game_id_)
    , participants(std::move(participants_))
{
}

LolApi::LolApi(utility::string_t api_key)
    : m_api_key(std::move(api_key))
{
}

LibRest::ExpectedTask<Summoner> LolApi::requestSummonerByName(const utility::string_t& region,
                                                              const utility::string_t& summoner_name) const
{
    return requestAPI(region, U("/summoner/v4/summoners/by-name/") + summoner_name, m_api_key).and_then(&parseSummoner);
}

LibRest::ExpectedTask<std::vector<utility::string_t>> LolApi::requestMatchlist(const utility::string_t& region,
                                                                               const utility::string_t& summoner_puuid,
                                                                               const MatchListOptions& options) const
{

    return requestAPI(region, U("/match/v5/matches/by-puuid/") + summoner_puuid + U("/ids"), m_api_key, {}, {},
                      toparams(options))
        .and_then(&parseMatchList);
}

auto LolApi::requestMatch(const utility::string_t& region, const utility::string_t& match_id) const
    -> LibRest::ExpectedTask<Match>
{
    ucout << U("LolApi::requestMatch(") << match_id << U(")\n");
    return requestAPI(region, U("/match/v5/matches/") + match_id, m_api_key).and_then(&parseMatch);
}

} // namespace LibLolAnalyzer
