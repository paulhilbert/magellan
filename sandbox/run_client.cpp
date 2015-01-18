#include <iostream>
#include <asio.hpp>
#include <asio/use_future.hpp>


int main (int argc, char const* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: run_client <host> <port>" << "\n";
            return 1;
        }

        asio::io_service io_service;

        asio::ip::tcp::socket s(io_service);
        asio::ip::tcp::resolver resolver(io_service);
        asio::connect(s, resolver.resolve({argv[1], argv[2]}));

        std::vector<float> values = { 2.5f, 1.f, 2.3f };
        std::cout << "before writing" << "\n";
        uint32_t length[1];
        length[0] = values.size();
        asio::write(s, asio::buffer(length));
        asio::write(s, asio::buffer(values));

        std::cout << "done sending" << "\n";
    } catch (std::system_error& e) {
        std::cerr << e.what() << "\n";
    }
}
