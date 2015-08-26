#include <iostream>
namespace magellan {

template <typename Session>
inline void
server::accept(asio::io_context& io_context, short port) {
    //using asio::ip::tcp;
    //accept<Session>(io_context, port, [](tcp::socket s) {
        //return std::make_shared<Session>(std::move(s));
    //});
    using asio::ip::tcp;
    asio::spawn(io_context, [&](asio::yield_context yield) {
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            asio::error_code ec;
            tcp::socket socket(io_context);
            acceptor.async_accept(socket, yield[ec]);
            if (!ec) {
                auto session = std::make_shared<Session>(std::move(socket));
                session->start();
            }
        }
    });
}

//template <typename Session, typename Func>
//void
//server::accept(asio::io_context& io_context, short port, Func&& factory) {
    //using asio::ip::tcp;
    //std::cout << port << "\n";
    //asio::spawn(io_context, [&](asio::yield_context yield) {
        //tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        //for (;;) {
            //std::cout << "accepting"
                      //<< "\n";
            //asio::error_code ec;
            //tcp::socket socket(io_context);
            //std::cout << "socket open: " << std::boolalpha << socket.is_open() << "\n";
            //acceptor.async_accept(socket, yield[ec]);
            //if (!ec) {
                //std::cout << "session"
                          //<< "\n";
                //auto session = factory(std::move(socket));
                //session->start();
            //} else {
                //std::cout << "failed accept" << "\n";
            //}
        //}
    //});
//}

} // magellan
