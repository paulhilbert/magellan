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
#include <asio/use_future.hpp>

#include <cereal/archives/json.hpp>

#include "future_monad.hpp"

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

        template <typename Handler>
        void connect(asio::io_service& io_service, const std::string& host, const std::string& service, Handler&& handler);

        template <typename Handler>
        static void start_accept_tcp(asio::ip::tcp::acceptor& acceptor, Handler&& handler);

        void finish();

        template <typename T, typename Handler>
        void async_write(const T& t, Handler handler);

        template <typename T>
        std::future<std::size_t> async_write(const T& t);

        template <typename T, typename Handler>
        void async_read(T& t, Handler handler);

        template <typename T>
        std::shared_future<T> async_read();

        template <typename T, typename Handler>
        void handle_read_header(const asio::error_code& e, T& t, std::tuple<Handler> handler);

        template <typename T, typename Handler>
        void handle_read_data(const asio::error_code& e, T& t, std::tuple<Handler> handler);

    protected:
        static constexpr int header_length = 16;

    protected:
        std::shared_future<std::array<char, header_length>> async_read_header_();
        std::shared_future<std::vector<char>> async_read_body_(std::array<char, header_length> header);
        template <typename T>
        std::shared_future<T> deserialize_body_(const std::vector<char>& data);


    private:
        asio::ip::tcp::socket socket_;
        //enum { header_length = 16 };
        std::string outbound_header_;
        std::string outbound_data_;
        char inbound_header_[header_length];
        std::vector<char> inbound_data_;
        std::shared_ptr<asio::io_service::work> work_;
};

#include "connection.ipp"

} // magellan

#endif // MAGELLAN_CONNECTION_HPP
