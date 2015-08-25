#include <server.hpp>
using asio::ip::tcp;

namespace pcl_compress {

server::server(asio::io_context& io_context, const tcp::endpoint& endpoint, compressed_cloud_t::ptr_t cloud)
    : io_context_(io_context), acceptor_(io_context, endpoint), pool_(new session_pool()), cloud_(cloud) {
    start_accept();
}

void
server::start_accept() {
    server_session::ptr_t new_session(new server_session(io_context_, pool_));
    acceptor_.async_accept(new_session->socket(),
                           std::bind(&server::handle_accept, this, new_session,
                                     std::placeholders::_1));
}

void
server::handle_accept(server_session::ptr_t session, const asio::error_code& error) {
    if (!error) {
        session->start(cloud_);
    }

    start_accept();
}

}  // pcl_compress
