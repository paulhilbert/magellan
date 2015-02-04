template <typename Kernel>
template <typename... KernelArgs>
inline server<Kernel>::server(asio::io_service& io_service, unsigned short port, KernelArgs&&... args) : acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), kernel_(new Kernel(std::forward<KernelArgs>(args)...)) {
    connection::start_accept_tcp(acceptor_, std::bind(&server::handle_accept, this, std::placeholders::_1, std::placeholders::_2));
}

template <typename Kernel>
inline void server<Kernel>::handle_accept(const asio::error_code& e, connection::ptr_t conn) {
    if (!e) {
        kernel_->accept(e, conn);
    }

    connection::start_accept_tcp(acceptor_, std::bind(&server::handle_accept, this, std::placeholders::_1, std::placeholders::_2));
}
