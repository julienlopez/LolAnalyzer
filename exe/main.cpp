#include <iostream>

#include <LolApi.hpp>

#include <expected_task/when_all.hpp>

using LibLolAnalyzer::LolApi;
using LibLolAnalyzer::Summoner;

namespace
{

struct MatchDetail
{
    std::size_t game_id;
    std::size_t game_duration;
    LolApi::Match::Participant details;
};

auto isParticipant(const utility::string_t& summoner_puuid)
{
    return [&summoner_puuid](const LolApi::Match::Participant& part) -> bool
    { return part.summoner_id == summoner_puuid; };
}

auto extractMatchDetailForSummoner(const utility::string_t& summoner_puuid)
{
    return [&](const LolApi::Match& match) -> MatchDetail
    {
        const auto it = std::ranges::find_if(match.participants, isParticipant(summoner_puuid));
        assert(it != end(match.participants));
        return {.game_id = match.game_id, .game_duration = match.game_duration, .details = *it};
    };
}

auto extractParticipantData(const utility::string_t& summoner_puuid)
{
    return [summoner_puuid](const std::vector<LolApi::Match>& matches) -> std::vector<MatchDetail>
    {
        std::vector<MatchDetail> res;
        std::ranges::transform(matches, std::back_inserter(res), extractMatchDetailForSummoner(summoner_puuid));
        return res;
    };
}

auto findMatchDetailsForSummoner(LolApi& api, const Summoner& summoner)
{
    return api.requestMatchlist(U("europe"), summoner.puuid(), {.type = utility::string_t{U("ranked")}, .count = 10})
        .and_then(
            [&api](const std::vector<utility::string_t>& matches)
            {
                std::vector<expected_task::expected_task<LolApi::Match>> tasks;
                for(const auto match : matches)
                    tasks.push_back(api.requestMatch(U("europe"), match));
                return expected_task::when_all(tasks);
            })
        .then_map(extractParticipantData(summoner.id()));
}

} // namespace

// int main(int argc, char* argv)
int wmain(int argc, wchar_t* argv[])
{
    if(argc != 2)
    {
        std::wcerr << "expected usage : LolAnalyzer <api_token>\n";
        return 1;
    }

    LolApi api(std::wstring{argv[1]});
    auto task = api.requestSummonerByName(U("euw1"), U("Haraelendil"))
                    .and_then(std::bind_front(&findMatchDetailsForSummoner, std::ref(api)))
                    .then_map(
                        [](const std::vector<MatchDetail> details)
                        {
                            ucout << details.size() << " matches found\n";
                            for(const auto& d : details)
                            {
                                ucout << d.game_duration << " : " << d.details.gold_earned << " | "
                                      << d.details.gold_earned << " ( " << d.details.kills << "/" << d.details.deaths
                                      << "/" << d.details.assists << ")\n";
                            }
                        })
                    .or_else([](const utility::string_t& error) { std::wcout << L"Error : " << error << std::endl; });

    try
    {
        task.wait();
    }
    catch(const std::exception& e)
    {
        std::cout << "Error exception:" << e.what() << std::endl;
    }

    std::cout << "done" << std::endl;
    return 0;
}
