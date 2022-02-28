#include <iostream>

#include <LolApi.hpp>

using LibLolAnalyzer::LolApi;
using LibLolAnalyzer::Summoner;

namespace
{

void displayMatchDetail(const LolApi::MatchDetailForSummoner& detail)
{
    ucout << detail.game_duration << " : " << detail.details.gold_earned << " | " << detail.details.gold_earned << " ( "
          << detail.details.kills << "/" << detail.details.deaths << "/" << detail.details.assists << ")\n";
}

void displayMatchDetails(const std::vector<LolApi::MatchDetailForSummoner>& details)
{
    ucout << details.size() << " matches found\n";
    std::ranges::for_each(details, &displayMatchDetail);
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
                    .and_then(std::bind_front(std::mem_fn(&LolApi::findMatchDetailsForSummoner), std::ref(api)))
                    .then_map(&displayMatchDetails)
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
