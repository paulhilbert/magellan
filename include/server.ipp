template <typename Kernel>
template <typename... KernelArgs>
inline server<Kernel>::server(asio::io_service& io_service, unsigned short port, KernelArgs&&... args) : acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), kernel_(new Kernel(std::forward<KernelArgs>(args)...)) {
    connection::ptr_t new_conn(new connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(), std::bind(&server::handle_accept, this, std::placeholders::_1, new_conn));
}

template <typename Kernel>
inline void server<Kernel>::handle_accept(const asio::error_code& e, connection::ptr_t conn) {
    if (!e) {
        kernel_->accept(e, conn);
    }

    connection::ptr_t new_conn(new connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(), std::bind(&server::handle_accept, this, std::placeholders::_1, new_conn));
}
