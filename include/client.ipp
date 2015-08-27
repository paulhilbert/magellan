#include <iostream>
namespace magellan {



template <typename Session>
inline void
client::connect(std::string host, short port) {
    using asio::ip::tcp;
    connect<Session>(host, port, [](tcp::socket s) {
        return std::make_shared<Session>(std::move(s));
    });
}

template <typename Session, typename Func>
inline void
client::connect(std::string host, short port, Func&& factory) {
    using asio::ip::tcp;
    asio::spawn(io_context_, [host, port, factory, this](asio::yield_context yield) {
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

        auto session = factory(std::move(socket));
        std::optional<std::chrono::milliseconds> expires = session->expiration();
        session->start(expires);
    });
}

} // magellan
