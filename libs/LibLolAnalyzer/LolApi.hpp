#pragma once

#include "ExpectedTask.hpp"

#include "summoner.hpp"

#include <cpprest/json.h>

namespace LibLolAnalyzer
{

class LolApi
{

public:
    static const utility::string_t s_summoner_api_version;
    static const utility::string_t s_static_data_api_version;

    LolApi(utility::string_t api_key);

    ~LolApi() = default;

    LibRest::ExpectedTask<Summoner> requestSummonerByName(const utility::string_t& region,
                                                          const utility::string_t& summoner_name) const;

    LibRest::ExpectedTask<web::json::value> requestMatchlist(const utility::string_t& region,
                                                             const utility::string_t& summoner_id) const;

private:
    const utility::string_t m_api_key;
};

} // namespace LibLolAnalyzer
