#include <packet.hpp>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace magellan {

packet::packet() : body_length_(0) {
}

packet::packet(const packet& other) {
    body_length_ = other.body_length_;
    memcpy(data_, other.data_, other.length());
}

packet& packet::operator=(const packet& other) {
    body_length_ = other.body_length_;
    memcpy(data_, other.data_, other.length());
    return *this;
}

packet::~packet() {
}


const packet::data_t* packet::data() const {
    return data_;
}

packet::data_t* packet::data() {
    return data_;
}

packet::type_t packet::packet_type() const {
    return type_;
}

void packet::set_packet_type(type_t packet_type) {
    type_ = packet_type;
}

packet::length_t packet::length() const {
    return header_length + body_length_;
}

const packet::data_t* packet::body() const {
    return data_ + header_length;
}

packet::data_t* packet::body() {
    return data_ + header_length;
}

packet::length_t packet::body_length() const {
    return body_length_;
}

void packet::set_body_length(length_t len) {
    body_length_ = len;
}

bool packet::decode_header() {
    memcpy((char*)&type_, data_, header_type_length);
    memcpy((char*)&body_length_, data_ + header_type_length, header_size_length);
    //char header[header_length + 1] = "";
    //strncat(header, data_, header_length);
    //int t, l;
    //sscanf(header, "%4d%4d", &t, &l);

    //type_ = static_cast<type_t>(t);
    //body_length_ = static_cast<length_t>(l);
    //if (body_length_ > max_body_length) {
        //body_length_ = 0;
        //return false;
    //}
    return true;
}

void packet::encode_header() {
    //char header[header_length + 1] = "";
    //sprintf(header, "%4d%4d", static_cast<int>(type_),
            //static_cast<int>(body_length_));
    //memcpy(data_, header, header_length);
    memcpy(data_, (const char*)&type_, header_type_length);
    memcpy(data_+header_type_length, (const char*)&body_length_, header_size_length);
}

std::vector<packet> packet::make_packets(const chunk_t& chunk) {
    std::vector<packet> packets;
    if (!chunk.size()) return packets;
    uint32_t overlap = chunk.size() % packet::max_body_length;
    uint32_t num_packets =
        chunk.size() / packet::max_body_length + (overlap ? 1 : 0);
    const typename chunk_t::value_type* input = chunk.data();
    for (uint32_t i = 0; i < num_packets; ++i) {
        packet p;

        bool last = i >= (num_packets - 1);

        packet::type_t packet_type =
            last ? packet::type_t::end_of_chunk : packet::type_t::part_of_chunk;
        p.set_packet_type(packet_type);

        packet::length_t body_length =
            last && overlap ? overlap : packet::max_body_length;
        p.set_body_length(body_length);

        p.encode_header();

        memcpy(p.body(), (const char*)input, body_length);
        input += body_length;

        packets.push_back(p);
    }

    return packets;
}

std::vector<packet> packet::make_packets(const stream_t& stream) {
    std::vector<packet> packets;
    if (!stream.size()) return packets;

    for (const auto& chunk : stream) {
        auto ps = make_packets(chunk);
        packets.insert(packets.end(), ps.begin(), ps.end());
    }
    packet last;
    last.set_packet_type(packet::type_t::end_of_stream);
    last.encode_header();
    packets.push_back(last);

    return packets;
}

} // magellan



