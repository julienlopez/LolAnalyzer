#include "RestServer.hpp"

RestServer::RestServer(utility::string_t url)
    : m_listener(std::move(url))
{
    m_listener.support(web::http::methods::GET, std::bind_front(&RestServer::handle_get, this));
}

pplx::task<void> RestServer::open()
{
    return m_listener.open();
}

pplx::task<void> RestServer::close()
{
    return m_listener.close();
}

void RestServer::handle_get(web::http::http_request message)
{

}

void RestServer::handle_error(pplx::task<void>& t)
{

}
