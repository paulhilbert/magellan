template <typename Buffer, typename Func>
inline void base_session::read(Buffer&& buffer, Func&& handler) {
    auto self(shared_from_this());
    socket_.async_read_some(
        std::forward<Buffer>(buffer),
        [this, self, handler] (const asio::error_code& ec, std::size_t length) {
            if (!ec) {
                handler(length);
            }
        }
    );
}

template <typename Buffer, typename Func>
inline void base_session::write(Buffer&& buffer, Func&& handler) {
    auto self(shared_from_this());
    asio::async_write(
        socket_,
        std::forward<Buffer>(buffer),
        [this, self, handler] (const asio::error_code& ec, std::size_t length) {
            if (!ec) {
                handler(length);
            }
        }
    );
}
