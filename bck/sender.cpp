#include <sender.hpp>

namespace pcl_compress {

sender::sender(asio::io_context& io_context) : session(io_context) {
}

sender::~sender() {
}

void
sender::write(const stream_t& stream) {
    auto self(shared_from_this());
    asio::spawn(strand_, [this, self] (asio::yield_context yield) {
        try {
            std::vector<packet> packets = packet::make_packets(current_stream_);

        } catch (std::exception& e) {
            socket_.close();
        }
    });
    if (pool_) pool_->add(shared_from_this());
    current_stream_ = stream;
    asio::post(io_context_,
               std::bind(&sender::do_write, this));
}

void sender::do_write() {
    bool write_in_progress = !write_queue_.empty();
    std::vector<packet> packets = packet::make_packets(current_stream_);
    write_queue_.insert(write_queue_.end(), packets.begin(), packets.end());
    current_stream_.clear();
    if (!write_in_progress) {
        asio::async_write(
            socket_, asio::buffer(write_queue_.front().data(),
                                  write_queue_.front().length()),
            std::bind(&sender::handle_write, this, std::placeholders::_1));
    }
}

void
sender::handle_write(const asio::error_code& error) {
    if (!error) {
        write_queue_.pop_front();
        if (!write_queue_.empty()) {
            asio::async_write(
                socket_, asio::buffer(write_queue_.front().data(),
                                      write_queue_.front().length()),
                std::bind(&sender::handle_write, this, std::placeholders::_1));
        } else {
            on_finished_write();
        }
    } else {
        if (pool_) pool_->remove(shared_from_this());
    }
}

}  // pcl_compress
