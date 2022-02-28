#pragma once

#include <cpprest/http_listener.h>

class RestServer
{
public:
    RestServer(utility::string_t url);
    
    ~RestServer() = default;

    pplx::task<void> open();

    pplx::task<void> close();

private:
    web::http::experimental::listener::http_listener m_listener;

    void handle_get(web::http::http_request message);

    void handle_error(pplx::task<void>& t);
};
