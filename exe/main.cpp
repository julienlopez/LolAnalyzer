#include <iostream>

#include <LolApi.hpp>

#include <expected_task/when_all.hpp>

using LibLolAnalyzer::LolApi;
using LibLolAnalyzer::Summoner;

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
                    .then_map([](const Summoner& summoner) { return summoner.puuid(); })
                    .and_then(
                        [&api](const utility::string_t& summoner_puuid) {
                            return api.requestMatchlist(U("europe"), summoner_puuid,
                                                        {.type = utility::string_t{U("ranked")}, .count = 2});
                        })
                    .and_then(
                        [&api](const std::vector<utility::string_t>& matches)
                        {
                            std::vector<expected_task::expected_task<LolApi::Match>> tasks;
                            for(const auto match : matches)
                                tasks.push_back(api.requestMatch(U("europe"), match));
                            return expected_task::when_all(tasks);
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
