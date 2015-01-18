template <class Session>
template <typename... Args>
inline typename server<Session>::ptr_t server<Session>::create(asio::io_service& io_service, short port, Args&&... args) {
    ptr_t instance(new server<Session>(io_service, port));
	instance->accept_connection<Args...>(std::forward<Args>(args)...);
    return instance;
}

template <class Session>
inline server<Session>::~server() {
}

template <class Session>
inline server<Session>::server(asio::io_service& io_service, short port)  : acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), socket_(io_service) {
}

template <class Session>
template <typename... Args>
inline void server<Session>::accept_connection(Args&&... args) {
	acceptor_.async_accept(
        socket_,
        [&] (std::error_code ec) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket_), std::forward<Args>(args)...)->start();
            }

            accept_connection(std::forward<Args>(args)...);
        }
    );
}

template <class Session, typename... Args>
inline void run_server(short port, Args&&... args) {
    asio::io_service io_service;
    auto s = server<Session>::create(io_service, port, std::forward<Args>(args)...);
    io_service.run();
}
