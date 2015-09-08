#include <iostream>
namespace magellan {

template <typename Session, typename... Args>
void
server::accept(short port, Args&&... args) {
    using asio::ip::tcp;
    asio::spawn(io_context_, [port, this, &args...](asio::yield_context yield) {
        tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            asio::error_code ec;
            tcp::socket socket(io_context_);
            acceptor.async_accept(socket, yield[ec]);
            if (!ec) {
                auto session = std::make_shared<Session>(std::move(socket), std::forward<Args>(args)...);
                std::optional<std::chrono::milliseconds> expires = session->expiration();
                session->start(expires);
            }
        }
    });
}

} // magellan
