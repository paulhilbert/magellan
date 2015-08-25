#include <connection.hpp>

#include <asio/use_future.hpp>

#include <lemonade/lemonade.hpp>
using namespace lemonade;

namespace magellan {

connection::connection(asio::io_service& io_service, const std::string& host, const std::string& service) : socket_(io_service) {
    std::future<void> future = connect_(io_service, host, service);
    future.get();
}

const asio::ip::tcp::socket&
connection::socket() const {
    return socket_;
}

asio::ip::tcp::socket&
connection::socket() {
    return socket_;
}

std::future<void>
connection::write(char data) {
    std::array<char, 1> ar = {{ data }};
    auto buffer = asio::buffer(ar);
    return chain(
        socket_.async_send(std::move(buffer), asio::use_future),
        [&] (std::size_t len) {
            if (len != 1) {
                throw std::runtime_error("error");
            }
            return mreturn<std::future>();
        }
    );
}

std::future<std::string>
connection::read() {
    std::array<char, 128> data;
    auto buffer = asio::buffer(data);
    return chain(
        socket_.async_receive(std::move(buffer), asio::use_future),
        [&] (std::size_t len) {
            std::string str(data.data(), len);
            return mreturn<std::future, std::string>(std::move(str));
        }
    );
}

connection::connection(asio::ip::tcp::acceptor& acceptor) : socket_(acceptor.get_io_service()) {
}

std::future<void>
connection::connect_(asio::io_service& io_service, const std::string& host, const std::string& service) {
    asio::ip::tcp::resolver resolver(io_service);
    return chain(
        resolver.async_resolve({asio::ip::tcp::v4(), host, service}, asio::use_future),
        [&] (asio::ip::tcp::resolver::iterator iter) {
            return mreturn<std::future, endpoint_t>(*iter);
        },
        [&] (auto endpoint) {
            return socket_.async_connect(endpoint, asio::use_future);
        }
    );
}


} // magellan
