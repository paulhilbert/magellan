inline connection::connection(asio::io_service& io_service) : socket_(io_service), work_(new asio::io_service::work(io_service)) {
}

inline asio::ip::tcp::socket& connection::socket() {
    return socket_;
}

template <typename Handler>
inline void connection::connect(asio::io_service& io_service, const std::string& host, const std::string& service, Handler&& handler) {
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(host, service);
    asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    asio::async_connect(socket_, endpoint_iterator, std::forward<Handler>(handler));
}

template <typename Handler>
inline void connection::start_accept_tcp(asio::ip::tcp::acceptor& acceptor, Handler&& handler) {
    std::shared_ptr<connection> conn(new connection(acceptor.get_io_service()));
    acceptor.async_accept(conn->socket(), std::bind(handler, std::placeholders::_1, conn));
}

inline void connection::finish() {
    work_.reset();
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

template <typename T>
inline std::future<std::size_t> connection::async_write(const T& t) {
    std::ostringstream archive_stream;
    {
        cereal::JSONOutputArchive archive(archive_stream);
        archive(t);
    }
    outbound_data_ = archive_stream.str();

    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex << outbound_data_.size();
    //if (!header_stream || header_stream.str().size() != header_length) {
        //asio::error_code error(asio::error::invalid_argument);
        //socket_.get_io_service().post(std::bind(handler, error));
        //return;
    //}
    outbound_header_ = header_stream.str();

    std::vector<asio::const_buffer> buffers;
    buffers.push_back(asio::buffer(outbound_header_));
    buffers.push_back(asio::buffer(outbound_data_));
    return asio::async_write(socket_, buffers, asio::use_future);
}

template <typename T, typename Handler>
inline void connection::async_read(T& t, Handler handler) {
    void (connection::*f)(const asio::error_code&, T&, std::tuple<Handler>) = &connection::handle_read_header<T, Handler>;
    asio::async_read(socket_, asio::buffer(inbound_header_), std::bind(f, this, std::placeholders::_1, std::ref(t), std::make_tuple(handler)));
}

template <typename T>
inline std::shared_future<T> connection::async_read() {
    try {
        return
            DO( header, this->async_read_header_(),
            DO( body, this->async_read_body_(header),
            this->deserialize_body_<T>(body)
            ));
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
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

inline std::shared_future<std::array<char, connection::header_length>> connection::async_read_header_() {
    std::array<char, header_length> header;
    auto future = asio::async_read(socket_, asio::buffer(header), asio::use_future).share();
    return DO(len, future, mreturn(header));
}

inline std::shared_future<std::vector<char>> connection::async_read_body_(std::array<char, header_length> header) {
    std::istringstream is(std::string(header.begin(), header.end()));
    std::size_t data_size = 0;
    is >> std::hex >> data_size;
    std::vector<char> data(data_size);
    auto future = asio::async_read(socket_, asio::buffer(data), asio::use_future).share();
    return DO(len, future, mreturn(data));
}

template <typename T>
inline std::shared_future<T> connection::deserialize_body_(const std::vector<char>& data) {
    return std::async(
        std::launch::async,
        [&] () {
            T t;
            std::string archive_data(&data[0], data.size());
            std::istringstream archive_stream(archive_data);
            {
                cereal::JSONInputArchive archive(archive_stream);
                archive(t);
            }
            return t;
        }
    ).share();
}
