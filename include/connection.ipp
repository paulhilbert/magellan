inline connection::connection(asio::io_service& io_service) : socket_(io_service) {
}

inline asio::ip::tcp::socket& connection::socket() {
    return socket_;
}

template <typename T, typename Handler>
inline void connection::async_write(const T& t, Handler handler) {
    std::ostringstream archive_stream;
    {
        cereal::JSONOutputArchive archive(archive_stream);
        archive(t);
    }
    outbound_data_ = archive_stream.str();

    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex << outbound_data_.size();
    if (!header_stream || header_stream.str().size() != header_length) {
        asio::error_code error(asio::error::invalid_argument);
        socket_.get_io_service().post(std::bind(handler, error));
        return;
    }
    outbound_header_ = header_stream.str();

    std::vector<asio::const_buffer> buffers;
    buffers.push_back(asio::buffer(outbound_header_));
    buffers.push_back(asio::buffer(outbound_data_));
    asio::async_write(socket_, buffers, handler);
}

template <typename T, typename Handler>
inline void connection::async_read(T& t, Handler handler) {
    void (connection::*f)(const asio::error_code&, T&, std::tuple<Handler>) = &connection::handle_read_header<T, Handler>;
    asio::async_read(socket_, asio::buffer(inbound_header_), std::bind(f, this, std::placeholders::_1, std::ref(t), std::make_tuple(handler)));
}

template <typename T, typename Handler>
inline void connection::handle_read_header(const asio::error_code& e, T& t, std::tuple<Handler> handler) {
    if (e) {
        std::get<0>(handler)(e);
    } else {
        // Determine the length of the serialized data.
        std::istringstream is(std::string(inbound_header_, header_length));
        std::size_t inbound_data_size = 0;
        if (!(is >> std::hex >> inbound_data_size)) {
            // Header doesn't seem to be valid. Inform the caller.
            asio::error_code error(asio::error::invalid_argument);
            std::get<0>(handler)(error);
            return;
        }

        // Start an asynchronous call to receive the data.
        inbound_data_.resize(inbound_data_size);
        void (connection::*f)(const asio::error_code&, T&, std::tuple<Handler>) = &connection::handle_read_data<T, Handler>;
        asio::async_read(socket_, asio::buffer(inbound_data_), std::bind(f, this, std::placeholders::_1, std::ref(t), handler));
    }
}

template <typename T, typename Handler>
inline void connection::handle_read_data(const asio::error_code& e, T& t, std::tuple<Handler> handler) {
    if (e) {
        std::get<0>(handler)(e);
    } else {
        try {
            std::string archive_data(&inbound_data_[0], inbound_data_.size());
            std::istringstream archive_stream(archive_data);
            {
                cereal::JSONInputArchive archive(archive_stream);
                archive(t);
            }
        } catch (std::exception& e) {
            asio::error_code error(asio::error::invalid_argument);
            std::get<0>(handler)(error);
            return;
        }

        std::get<0>(handler)(e);
    }
}
