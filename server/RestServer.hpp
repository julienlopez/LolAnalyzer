#pragma once

#include <memory>

#include <cpprest/details/basic_types.h>

#include <pplx/pplxtasks.h>

namespace LibLolAnalyzer
{
class LolApi;
}

namespace Impl
{
class RestServer;
}

class RestServer
{
public:
    RestServer(LibLolAnalyzer::LolApi& api, utility::string_t url);

    ~RestServer();

    pplx::task<void> open();

    pplx::task<void> close();

private:
    std::unique_ptr<Impl::RestServer> m_pimpl;
};
