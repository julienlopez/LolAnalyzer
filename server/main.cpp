#include <iostream>
#include <memory>

#include "RestServer.hpp"

#include "LolApi.hpp"

#include <cpprest/uri_builder.h>

std::unique_ptr<RestServer> on_initialize(LibLolAnalyzer::LolApi& api, const utility::string_t& address)
{
    web::uri_builder uri(address);

    auto addr = uri.to_uri().to_string();
    auto server = std::make_unique<RestServer>(std::ref(api), std::move(addr));
    server->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return server;
}

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{

    if(argc != 2 && argc != 3)
    {
        std::wcerr << "expected usage : LolAnalyzerServer <api_token> <port>\n";
        return 1;
    }
    utility::string_t port = U("8080");
    if(argc == 3)
    {
        port = argv[2];
    }

    LibLolAnalyzer::LolApi api(std::wstring{argv[1]});
    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);

    auto server = on_initialize(api, address);
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    server->close().wait();
    return 0;
}
