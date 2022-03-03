#include "RestServer.hpp"

#include "RequestSplitter.hpp"

#include "LolApi.hpp"

#include <cpprest/http_listener.h>

using LibLolAnalyzer::LolApi;

namespace Impl
{

web::json::value buildGenericError(utility::string_t error)
{
    return web::json::value::object({{U("error_message"), web::json::value::string(std::move(error))}});
}

struct Reply
{
    Reply()
        : code()
        , payload()
    {
    }

    Reply(web::json::value value)
        : code(web::http::status_codes::OK)
        , payload(std::move(value))
    {
    }

    Reply(utility::string_t error)
        : code(web::http::status_codes::InternalError)
        , payload(buildGenericError(error))
    {
    }

    Reply(web::http::status_code code_, utility::string_t error)
        : code(code_)
        , payload(buildGenericError(error))
    {
    }

    web::http::status_code code;
    web::json::value payload;
};

auto replyTo(web::http::http_request message)
{
    return [=](pplx::task<Reply> reply)
    {
        const auto res = reply.get();
        try
        {
            message.reply(res.code, res.payload);
        }
        catch(...)
        {
            message.reply(web::http::status_codes::InternalError, buildGenericError(U("Unexpected error")));
        }
    };
}

Reply errorToReply(utility::string_t error)
{
    return std::move(error);
}

Reply toReply(web::json::value json)
{
    return std::move(json);
}

Reply replyUnknownEndpoint(const web::http::uri& uri)
{
    return {web::http::status_codes::NotFound, U("unknown endpoint : ") + uri.to_string()};
}

web::json::value matchDetailToJson(const LolApi::MatchDetailForSummoner& detail)
{
    return web::json::value::object(
        {{U("game_id"), web::json::value(detail.game_id)},
         {U("game_duration"), web::json::value(detail.game_duration)},
         {U("kills"), web::json::value(detail.details.kills)},
         {U("assists"), web::json::value(detail.details.assists)},
         {U("deaths"), web::json::value(detail.details.deaths)},
         {U("lane"), web::json::value::string(detail.details.lane)},
         {U("summoner_id"), web::json::value::string(detail.details.summoner_id)},
         {U("champion_id"), web::json::value(detail.details.champion_id)},
         {U("champion_name"), web::json::value::string(detail.details.champion_name)},
         {U("team_position"), web::json::value::string(detail.details.team_position)},
         {U("gold_earned"), web::json::value(detail.details.gold_earned)},
         {U("total_damage_dealt"), web::json::value(detail.details.total_damage_dealt)},
         {U("total_damage_dealt_to_champions"), web::json::value(detail.details.total_damage_dealt_to_champions)},
         {U("total_damage_taken"), web::json::value(detail.details.total_damage_taken)},
         {U("total_minions_killed"), web::json::value(detail.details.total_minions_killed)},
         {U("vision_score"), web::json::value(detail.details.vision_score)},
         {U("vision_wards_bought_in_game"), web::json::value(detail.details.vision_wards_bought_in_game)},
         {U("wards_killed"), web::json::value(detail.details.wards_killed)},
         {U("wards_placed"), web::json::value(detail.details.wards_placed)}});
}

web::json::value matchDetailsToJson(const std::vector<LolApi::MatchDetailForSummoner>& details)
{
    std::vector<web::json::value> res;
    std::ranges::transform(details, std::back_inserter(res), &matchDetailToJson);
    return web::json::value::array(std::move(res));
}

class RestServer
{
public:
    RestServer(LibLolAnalyzer::LolApi& api, utility::string_t url)
        : m_api(api)
        , m_listener(std::move(url))
    {
        m_listener.support(web::http::methods::GET, std::bind_front(&RestServer::handle_get, this));
    }

    pplx::task<void> open()
    {
        return m_listener.open();
    }

    pplx::task<void> close()
    {
        return m_listener.close();
    }

private:
    LolApi& m_api;
    web::http::experimental::listener::http_listener m_listener;

    void handle_get(web::http::http_request message)
    {
        // ucout << U("RestServer::handle_get") << message.relative_uri().to_string() << std::endl;
        const auto res = LibRest::RequestSplitter::parse(message.relative_uri());
        // ucout << "path.size() = " << res.paths.size() << std::endl;
        // for(const auto& p : res.paths)
        //     ucout << p << ",";
        // ucout << "params.size() = " << res.parameters.size() << std::endl;
        // for(const auto& p : res.parameters)
        //     ucout << p.first << " => " << p.second << ",";
        if(not res.paths.empty() && res.paths.front() == U("analyze"))
            analyze(res)
                .then_map(&toReply)
                .then_return_value_or_convert_error_to_value(&errorToReply)
                .then(replyTo(message));
        else
            pplx::task_from_result(replyUnknownEndpoint(message.relative_uri())).then(replyTo(message));
    }

    LibRest::ExpectedTask<web::json::value> analyze(const LibRest::RequestSplitter::Result& query)
    {
        if(query.paths.size() != 3)
        {
            utility::ostringstream_t ss;
            ss << U("invalid analyze parameters (") << query.paths.size() << U(")");
            return tl::make_unexpected(ss.str());
        }
        return m_api.requestSummonerByName(U("euw1"), U("Haraelendil"))
            .and_then(std::bind_front(std::mem_fn(&LolApi::findMatchDetailsForSummoner), std::ref(m_api)))
            .then_map(&matchDetailsToJson);
    }
};

} // namespace Impl

RestServer::RestServer(LibLolAnalyzer::LolApi& api, utility::string_t url)
    : m_pimpl(std::make_unique<Impl::RestServer>(std::ref(api), std::move(url)))
{
}

RestServer::~RestServer() = default;

pplx::task<void> RestServer::open()
{
    return m_pimpl->open();
}

pplx::task<void> RestServer::close()
{
    return m_pimpl->close();
}