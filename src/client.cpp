#include <client.hpp>

namespace magellan {

client::client() {}

client::~client() {}

void
client::run() {
    io_context_.run();
}

}  // magellan
