#ifndef MAGELLAN_STREAM_PACKET_HPP_
#define MAGELLAN_STREAM_PACKET_HPP_

#include "packet.hpp"

namespace magellan {

typedef enum class stream_type_ : char {
    part_of_chunk,
    end_of_chunk,
    end_of_stream
} stream_type_t;

template <int MaxBodyLength>
using stream_packet = packet<stream_type_t, MaxBodyLength>;

template <int MaxBodyLength, typename T>
std::vector<stream_packet<MaxBodyLength>> make_chunk_packets(T&& v);

template <int MaxBodyLength, typename InputIterator>
std::vector<stream_packet<MaxBodyLength>> make_stream_packets(InputIterator first, InputIterator last);

template <typename T, int MaxBodyLength>
T reconstruct_chunk(const std::vector<stream_packet<MaxBodyLength>>& packets);

} // magellan

#include "stream_packet.ipp"

#endif /* MAGELLAN_STREAM_PACKET_HPP_ */
