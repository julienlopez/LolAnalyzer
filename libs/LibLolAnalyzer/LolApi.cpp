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

    LibRest::ExpectedTask<web::json::value>
    requestAPI(const std::wstring& region, const std::wstring& request, const std::wstring& api_key,
               const std::wstring& option = {}, const std::wstring prefix_region = {},
               const std::map<std::wstring, std::wstring>& additional_parameters = {})
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
} // namespace

LolApi::LolApi(std::wstring api_key)
    : m_api_key(std::move(api_key))
{
}

LibRest::ExpectedTask<Summoner> LolApi::requestSummonerByName(const std::wstring& region,
                                                              const std::wstring& summoner_name) const
{
    return requestAPI(region, U("/summoner/v4/summoners/by-name/") + summoner_name, m_api_key)
        .and_then(&parseSummoner);
}

LibRest::ExpectedTask<web::json::value> LolApi::requestMatchlist(const std::wstring& region,
                                                                 const utility::string_t& summoner_puuid) const
{
    return requestAPI(region, U("/match/v5/matches/by-puuid/") + summoner_puuid + U("/ids"), m_api_key);
}

} // namespace LibLolAnalyzer
