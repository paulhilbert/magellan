#include <server.hpp>

namespace magellan {

server::server() {}

server::~server() {}

void
server::run() {
    io_context_.run();
}

}  // magellan
