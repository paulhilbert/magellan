#include <iostream>
#include <memory>
#include <asio.hpp>
#include <asio/spawn.hpp>
using asio::ip::tcp;

#include <server.hpp>

class container_session : public magellan::session {
    public:
        typedef std::shared_ptr<container_session>       ptr;
        typedef std::weak_ptr<container_session>         wptr;
        typedef std::shared_ptr<const container_session> const_ptr;
        typedef std::weak_ptr<const container_session>   const_wptr;

    public:
        container_session(tcp::socket socket)
            : magellan::session(std::move(socket)) {}

        virtual ~container_session() {}

    protected:
        void perform(asio::ip::tcp::socket& s, asio::yield_context& yc) {
            std::vector<uint32_t> data(400);
            std::iota(data.begin(), data.end(), 0);

            send_chunk<512>(data, s, yc);

            //char data[128];
            //for (;;) {
                //std::size_t n = s.async_read_some(asio::buffer(data), yc);
                //asio::async_write(s, asio::buffer(data, n), yc);
            //}
        }
};


int main (int argc, char const* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: echo_server <port>\n";
            return 1;
        }

        magellan::server server;
        server.accept<container_session>(9003);
        server.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
