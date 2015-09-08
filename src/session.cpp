#include <session.hpp>

using asio::ip::tcp;

namespace magellan {

session::session(tcp::socket socket) : socket_(std::move(socket)), strand_(socket_.get_io_context()), timer_(socket_.get_io_context()) {
}

tcp::socket&
session::socket() {
    return socket_;
}

const tcp::socket&
session::socket() const {
    return socket_;
}

asio::io_context&
session::context() {
    return socket_.get_io_context();
}

std::optional<std::chrono::milliseconds>
session::expiration() const {
    return std::nullopt;
}

void
session::perform(tcp::socket& s, asio::yield_context) {
    s.close();
}

}  // magellan
