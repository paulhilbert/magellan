#include <iostream>
#include <memory>
#include <asio.hpp>
#include <asio/spawn.hpp>
using asio::ip::tcp;

#include <server.hpp>

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
            char data[128];
            for (;;) {
                std::size_t n = s.async_read_some(asio::buffer(data), yc);
                asio::async_write(s, asio::buffer(data, n), yc);
            }
        }
};


int main (int argc, char const* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: echo_server <port>\n";
            return 1;
        }

        magellan::server server;
        server.accept<echo_session>(9003);
        server.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
