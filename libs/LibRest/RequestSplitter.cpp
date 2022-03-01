#include "RequestSplitter.hpp"

namespace LibRest::RequestSplitter
{

Result parse(const utility::string_t& uri)
{
    auto paths = web::uri::split_path(uri);
    if(paths.empty()) return {};
    auto last = paths.back();
    const auto pos = last.find('?');
    if(pos == utility::string_t::npos) return {paths, {}};
    paths.back() = last.substr(0, pos);
    const auto params = web::uri::split_query(last.substr(pos + 1));
    return {paths, params};
}

Result parse(const web::uri& uri)
{
    return parse(uri.to_string());
}

} // namespace LibRest::RequestSplitter