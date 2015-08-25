#include <packet_receiver.hpp>
#include <iostream>
#include <cstring>

namespace pcl_compress {

packet_receiver::packet_receiver() : end_of_stream_(false) {
}

packet_receiver::~packet_receiver() {
}

stde::optional<chunk_t> packet_receiver::insert(const packet& packet) {
    if (packet.packet_type() == packet::type_t::end_of_stream) {
        end_of_stream_ = true;
        return stde::nullopt;
    }

    const packet::data_t* data = packet.body();
    chunk_t values(packet.body_length());
    memcpy((char*)values.data(), data, packet.body_length());
    chunk_.insert(chunk_.end(), values.begin(), values.end());

    stde::optional<chunk_t> chunk = stde::nullopt;
    if (packet.packet_type() == packet::type_t::end_of_chunk) {
        chunk = chunk_;
        chunk_.clear();
    }

    return chunk;
}

bool packet_receiver::end_of_stream() const {
    return end_of_stream_;
}

void packet_receiver::reset() {
    end_of_stream_ = false;
    chunk_.clear();
}

} // pcl_compress
