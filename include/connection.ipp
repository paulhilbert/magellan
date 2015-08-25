template <typename Rep, typename Period>
inline
connection::connection(asio::io_service& io_service, const std::string& host, const std::string& service, std::chrono::duration<Rep, Period> timeout) : socket_(io_service) {
    std::future<void> future = connect_(io_service, host, service);
    if (future.wait_for(timeout.get()) != std::future_status::ready) {
        throw std::runtime_error("Establishing connection timed out.");
    }
}

template <typename T>
inline std::future<void> connection::async_write(const T& t) {
    std::ostringstream archive_stream;
    {
        cereal::JSONOutputArchive archive(archive_stream);
        archive(t);
    }
    std::string data = archive_stream.str();

    return lemonade::chain(
        lemonade::mreturn<std::future, std::string>(std::move(data)),
        [&] (std::string body) {
            std::ostringstream header_stream;
            header_stream << std::setw(header_length) << std::hex << outbound_data_.size();
            if (!header_stream || header_stream.str().size() != header_length) {
                throw std::runtime_error("Data size exceeds representable size in header.");
            }
            std::string header = header_stream.str();
            return lemonade::mreturn<std::future, std::tuple<std::string, std::string>>(std::make_tuple(header, body));
        },
        [&] (std::tuple<std::string, std::string> data) {
            std::string header, body;
            std::tie(header, body) = data;
            std::vector<asio::const_buffer> buffers;
            buffers.push_back(asio::buffer(header));
            buffers.push_back(asio::buffer(body));
            std::cout << "write data" << "\n";
            return asio::async_write(socket_, buffers, asio::use_future);
        },
        [&] (std::size_t) {
            std::cout << "wrote data" << "\n";
            return lemonade::mreturn<std::future>();
        }
    );
}

template <typename T>
inline std::future<T> connection::async_read() {
    typedef std::array<char, header_length> header_t;

    return lemonade::chain(
        lemonade::mreturn<std::future>(header_t()),
        [&] (header_t header) {
            std::cout << "start header" << "\n";
            return lemonade::chain(
                asio::async_read(socket_, asio::buffer(header), asio::use_future),
                [&] (std::size_t len_read) {
                    std::cout << "got header" << "\n";
                    if (len_read != header_length) throw std::runtime_error("Could not receive header");
                    return lemonade::mreturn<std::future>(std::move(header));
                }
            );
        },
        [&] (header_t header) {
            std::istringstream is(std::string(header.begin(), header.end()));
            std::size_t data_size = 0;
            is >> std::hex >> data_size;
            return lemonade::chain(
                lemonade::mreturn<std::future>(std::vector<char>(data_size)),
                [&] (std::vector<char> data_vec) {
                    return lemonade::chain(
                        asio::async_read(socket_, asio::buffer(data_vec), asio::use_future),
                        [&] (std::size_t len_read) {
                            if (data_vec.size() != len_read) throw std::runtime_error("Could not receive expected body");
                            return lemonade::mreturn<std::future>(std::move(data_vec));
                        }
                    );
                }
            );
        },
        [&] (std::vector<char> body_data) {
            T t;
            std::string archive_data(&body_data[0], body_data.size());
            std::istringstream archive_stream(archive_data);
            {
                cereal::JSONInputArchive archive(archive_stream);
                archive(t);
            }
            return lemonade::mreturn<std::future>(std::move(t));
        }
    );
}

template <typename Handler>
inline void connection::start_accept_tcp(asio::ip::tcp::acceptor& acceptor, Handler&& handler) {
    std::shared_ptr<connection> conn(new connection(acceptor));
    acceptor.async_accept(conn->socket(), std::bind(handler, std::placeholders::_1, conn));
}




