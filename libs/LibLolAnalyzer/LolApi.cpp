#include "lolapi.hpp"

#include "JsonParser.hpp"
#include "Result.hpp"
#include "Utils.hpp"

#include <boost/hof/construct.hpp>

#include <cpprest/http_client.h>

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

} // namespace

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

} // namespace LibLolAnalyzer
