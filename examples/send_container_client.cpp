#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <asio.hpp>
#include <asio/spawn.hpp>
using asio::ip::tcp;

#include <client.hpp>

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
            typedef std::vector<uint32_t> data_t;
            auto data = receive_chunk<512, data_t>(s, yc);
            std::cout << "Received " << data->size() << " vector elements" << "\n";
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
        client.connect<container_session>(argv[1], port);
        client.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
