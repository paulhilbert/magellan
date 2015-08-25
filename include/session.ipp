namespace magellan {

template <typename Func>
inline void
session::async_do(Func&& f) {
    auto self(shared_from_this());
    asio::spawn(strand_, [this, self, f](asio::yield_context yield) {
        try {
            f(std::ref(socket_), std::ref(yield));
        } catch (std::exception& e) {
            socket_.close();
        }
    });
}

} // magellan
