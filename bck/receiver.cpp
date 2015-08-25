#include <receiver.hpp>

namespace pcl_compress {

receiver::receiver(asio::io_context& io_context, session_pool::ptr_t pool) : session(io_context, pool) {
}

receiver::~receiver() {
}

void
receiver::read(const asio::error_code& error) {
    if (!error) {
        asio::async_read(
            socket_, asio::buffer(read_packet_.data(), packet::header_length),
            std::bind(&receiver::handle_read_header, this,
                      std::placeholders::_1));
    }
}

void
receiver::handle_read_header(const asio::error_code& error) {
    if (!error && read_packet_.decode_header()) {
        asio::async_read(
            socket_,
            asio::buffer(read_packet_.body(), read_packet_.body_length()),
            std::bind(&receiver::handle_read_body, this, std::placeholders::_1));
    } else {
        do_close();
    }
}

void
receiver::handle_read_body(const asio::error_code& error) {
    if (!error) {
        if (auto c = receiver_.insert(read_packet_)) {
            on_chunk_ready(*c);
        }
        if (receiver_.end_of_stream()) {
            on_stream_ready();
            receiver_.reset();
        }
        asio::async_read(
            socket_, asio::buffer(read_packet_.data(), packet::header_length),
            std::bind(&receiver::handle_read_header, this,
                      std::placeholders::_1));
    } else {
        do_close();
    }
}

}  // pcl_compress
