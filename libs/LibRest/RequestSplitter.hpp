#pragma once

#include <map>
#include <vector>

#include <cpprest/base_uri.h>
#include <cpprest/details/basic_types.h>

namespace LibRest::RequestSplitter
{

struct Result
{
    std::vector<utility::string_t> paths;
    std::map<utility::string_t, utility::string_t> parameters;
};

Result parse(const utility::string_t& uri);

Result parse(const web::uri& uri);

} // namespace LibRest::RequestSplitter