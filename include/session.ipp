namespace magellan {

template <typename Func, typename Period>
inline void
session::async_do(Func&& f, opt_duration<Period> expiration) {
    auto self(shared_from_this());
    asio::spawn(strand_, [this, self, f, expiration](asio::yield_context yield) {
        try {
            if (expiration) {
                timer_.expires_from_now(*expiration);
            }
            f(std::ref(socket_), std::ref(yield));
        } catch (std::exception& e) {
            socket_.close();
            timer_.cancel();
        }
    });

    if (expiration) {
        asio::spawn(strand_, [this, self](asio::yield_context yield) {
            while (socket_.is_open()) {
                asio::error_code ignored_ec;
                timer_.async_wait(yield[ignored_ec]);
                if (timer_.expires_from_now() <= duration<Period>(0))
                    socket_.close();
            }
        });
    }
}

template <typename Period>
inline void
session::start(opt_duration<Period> expiration) {
    async_do([this](asio::ip::tcp::socket& s, asio::yield_context& yc) {
        perform(s, yc);
    }, expiration);
}

} // magellan
