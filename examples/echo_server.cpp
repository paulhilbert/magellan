#include <iostream>
#include <server.hpp>

using asio::ip::tcp;

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
            std::cout << "test" << "\n";
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

        asio::io_context io_context;
        magellan::server server;

        server.accept<echo_session>(io_context, 9003);
        //server.run();


        //asio::spawn(io_context, [&](asio::yield_context yield) {
            //tcp::acceptor acceptor(
                //io_context, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));

            //for (;;) {
                //asio::error_code ec;
                //tcp::socket socket(io_context);
                //acceptor.async_accept(socket, yield[ec]);
                //if (!ec) {
                    //auto session =
                        //std::make_shared<echo_session>(std::move(socket));
                    //session->start();
                //}
            //}
        //});

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
