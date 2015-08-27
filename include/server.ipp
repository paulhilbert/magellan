#include <iostream>
namespace magellan {

template <typename Session>
inline void
server::accept(short port) {
    using asio::ip::tcp;
    accept<Session>(port, [](tcp::socket s) {
        return std::make_shared<Session>(std::move(s));
    });
    //using asio::ip::tcp;
    //asio::spawn(io_context, [port, &io_context](asio::yield_context yield) {
        //tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        //for (;;) {
            //asio::error_code ec;
            //tcp::socket socket(io_context);
            //acceptor.async_accept(socket, yield[ec]);
            //if (!ec) {
                //auto session = std::make_shared<Session>(std::move(socket));
                //session->start();
            //}
        //}
    //});
}

template <typename Session, typename Func>
void
server::accept(short port, Func&& factory) {
    using asio::ip::tcp;
    asio::spawn(io_context_, [port, factory, this](asio::yield_context yield) {
        tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            asio::error_code ec;
            tcp::socket socket(io_context_);
            acceptor.async_accept(socket, yield[ec]);
            if (!ec) {
                auto session = factory(std::move(socket));
                std::optional<std::chrono::milliseconds> expires = session->expiration();
                session->start(expires);
            }
        }
    });
}

} // magellan
