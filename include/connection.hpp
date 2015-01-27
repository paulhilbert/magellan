#ifndef MAGELLAN_CONNECTION_HPP
#define MAGELLAN_CONNECTION_HPP

#include <memory>
#include <functional>
#include <tuple>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>

#include <asio.hpp>

#include <cereal/archives/json.hpp>

namespace magellan {


class connection {
    public:
        typedef std::shared_ptr<connection>       ptr_t;
        typedef std::weak_ptr<connection>         wptr_t;
        typedef std::shared_ptr<const connection> const_ptr_t;
        typedef std::weak_ptr<const connection>   const_wptr_t;

    public:
        connection(asio::io_service& io_service);

        asio::ip::tcp::socket& socket();

        template <typename T, typename Handler>
        void async_write(const T& t, Handler handler);

        template <typename T, typename Handler>
        void async_read(T& t, Handler handler);

        template <typename T, typename Handler>
        void handle_read_header(const asio::error_code& e, T& t, std::tuple<Handler> handler);

        template <typename T, typename Handler>
        void handle_read_data(const asio::error_code& e, T& t, std::tuple<Handler> handler);

    private:
        asio::ip::tcp::socket socket_;
        enum { header_length = 16 };
        std::string outbound_header_;
        std::string outbound_data_;
        char inbound_header_[header_length];
        std::vector<char> inbound_data_;
};

#include "connection.ipp"

} // magellan

#endif // MAGELLAN_CONNECTION_HPP
