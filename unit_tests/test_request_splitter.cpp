#include <catch2/catch.hpp>

#include "RequestSplitter.hpp"

using namespace LibRest::RequestSplitter;

TEST_CASE("RequestSplitter")
{
    SECTION("path alone")
    {
        const utility::string_t input = U("/analyze/euw1/summoner");
        const auto res = parse(input);
        REQUIRE(res.paths.size() == 3);
        CHECK(res.paths[0] == U("analyze"));
        CHECK(res.paths[1] == U("euw1"));
        CHECK(res.paths[2] == U("summoner"));

        REQUIRE(res.parameters.empty());
    }

    SECTION("single path with params")
    {
        const utility::string_t input = U("/summoner?type=ranked");
        const auto res = parse(input);
        REQUIRE(res.paths.size() == 1);
        CHECK(res.paths.front() == U("summoner"));

        REQUIRE(res.parameters.size() == 1);
        CHECK(res.parameters.begin()->first == U("type"));
        CHECK(res.parameters.begin()->second == U("ranked"));
    }

    SECTION("multiple path with params")
    {
        const utility::string_t input = U("/analyze/euw1/summoner?type=ranked");
        const auto res = parse(input);
        REQUIRE(res.paths.size() == 3);
        CHECK(res.paths[0] == U("analyze"));
        CHECK(res.paths[1] == U("euw1"));
        CHECK(res.paths[2] == U("summoner"));
        
        REQUIRE(res.parameters.size() == 1);
        CHECK(res.parameters.begin()->first == U("type"));
        CHECK(res.parameters.begin()->second == U("ranked"));
    }

}