#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <asio.hpp>
#include <asio/spawn.hpp>
using asio::ip::tcp;

#include <client.hpp>

enum { max_length = 1024 };

class echo_session : public magellan::session {
    public:
        typedef std::shared_ptr<echo_session>       ptr;
        typedef std::weak_ptr<echo_session>         wptr;
        typedef std::shared_ptr<const echo_session> const_ptr;
        typedef std::weak_ptr<const echo_session>   const_wptr;

    public:
        echo_session(tcp::socket socket)
            : magellan::session(std::move(socket)) {}

        virtual ~echo_session() {}

    protected:
        void perform(asio::ip::tcp::socket& s, asio::yield_context& yc) {
            std::cout << "Enter message: ";
            char request[max_length];
            std::cin.getline(request, max_length);
            size_t request_length = std::strlen(request);
            asio::async_write(s, asio::buffer(request, request_length), yc);

            char reply[max_length];
            size_t reply_length =
                asio::async_read(s, asio::buffer(reply, request_length), yc);
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
};


int main (int argc, char const* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: echo_client <host> <port>\n";
            return 1;
        }

        short port = std::atoi(argv[2]);
        magellan::client client;
        client.connect<echo_session>(argv[1], port);
        client.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
