#include <session.hpp>

using asio::ip::tcp;

namespace magellan {

session::session(tcp::socket socket) : socket_(std::move(socket)), strand_(socket_.get_io_context()) {
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

void session::start() {
    async_do([this] (tcp::socket& s, asio::yield_context& yc) {
        perform(s, yc);
    });
}

void session::perform(tcp::socket& s, asio::yield_context&) {
    s.close();
}

}  // magellan
