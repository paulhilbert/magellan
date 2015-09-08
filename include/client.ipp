#include <iostream>
namespace magellan {


template <typename Session, typename... Args>
inline void
client::connect(std::string host, short port, Args&&... args) {
    using asio::ip::tcp;
    asio::spawn(io_context_, [host, port, this, &args...](asio::yield_context yield) {
        tcp::resolver resolver(io_context_);
        asio::error_code ec;

        asio::ip::tcp::socket socket(io_context_);

        auto endpoints = resolver.async_resolve(host, std::to_string(port), yield[ec]);
        if (ec) {
            throw std::runtime_error("Unable to resolve to \"" + host + ":"+std::to_string(port)+"\"");
        }

        asio::async_connect(socket, endpoints, yield[ec]);
        if (ec) {
            throw std::runtime_error("Unable to connect to \"" + host + ":"+std::to_string(port)+"\"");
        }

        //auto session = factory(std::move(socket));
        auto session = std::make_shared<Session>(std::move(socket), std::forward<Args>(args)...);
        std::optional<std::chrono::milliseconds> expires = session->expiration();
        session->start(expires);
    });
}

} // magellan
