#include <iostream>
#include <memory>

#include "RestServer.hpp"

// using namespace web;
// using namespace http;
// using namespace utility;

std::unique_ptr<RestServer> on_initialize(const utility::string_t& address)
{

    web::uri_builder uri(address);

    auto addr = uri.to_uri().to_string();
    auto server = std::make_unique<RestServer>(addr);
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
    utility::string_t port = U("34568");
    if(argc == 2)
    {
        port = argv[1];
    }

    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);

    auto server = on_initialize(address);
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    server->close().wait();
    return 0;
}
