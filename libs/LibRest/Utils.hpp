#pragma once

#include "Result.hpp"

#include <cpprest/http_msg.h>

namespace LibRest::Utils
{

bool isError(const web::http::status_code status);

/**
 * @brief used for easier chaining and error management
 */
auto extractJson()
{
    return [](pplx::task<web::http::http_response> message_task)
    {
        return pplx::create_task(
            [task = std::move(message_task)]() -> Result<web::json::value>
            {
                try
                {
                    auto res = task.get();
                    if(isError(res.status_code()))
                        return tl::make_unexpected(U("Request failed :") + res.extract_string().get());
                    else
                        return res.extract_json().get();
                }
                catch(const std::exception& ex)
                {
                    utility::ostringstream_t o;
                    o << U("Unable to parse json :") << ex.what();
                    return tl::make_unexpected(o.str());
                }
            });
    };
}

} // namespace LibRest::Utils
