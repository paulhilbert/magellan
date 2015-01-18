#include <base_session.hpp>

namespace magellan {

base_session::base_session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {
}

base_session::~base_session() {
}

} // magellan
