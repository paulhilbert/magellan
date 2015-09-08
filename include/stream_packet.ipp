#include <sstream>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>

namespace magellan {

template <int MaxBodyLength, typename T>
inline std::vector<stream_packet<MaxBodyLength>>
make_chunk_packets(T&& v) {
    typedef stream_packet<MaxBodyLength> packet_t;

	std::stringstream sstr;
	{
	    cereal::BinaryOutputArchive	ar(sstr);
        ar(v);
	}

    uint32_t chunk_length = sstr.tellp();
    sstr.seekg(0);

    std::vector<packet_t> packets;
    if (!chunk_length) return packets;
    uint32_t  overlap = chunk_length % MaxBodyLength;
    uint32_t  num_packets =
       chunk_length / MaxBodyLength + (overlap ? 1 : 0);
    for (uint32_t i = 0; i < num_packets; ++i) {
        packet_t  p;

        bool last = i >= (num_packets - 1);

        stream_type_t  packet_type =
           last ? stream_type_t::end_of_chunk : stream_type_t::part_of_chunk;
        p.header() = packet_type;

        typename packet_t::length_t  body_length =
           last && overlap ? overlap : MaxBodyLength;
        p.set_body_length(body_length);

        p.encode();

        sstr.read((char*)p.body(), body_length);

        packets.push_back(p);
    }

    return packets;
}

template <int MaxBodyLength, typename InputIterator>
inline std::vector<stream_packet<MaxBodyLength>>
make_stream_packets(InputIterator first, InputIterator last) {
    typedef stream_packet<MaxBodyLength> packet_t;
    std::vector<packet_t> packets;
    for(; first != last; ++first) {
        auto chunk = make_chunk_packets<MaxBodyLength>(*first);
        packets.insert(packets.end(), chunk.begin(), chunk.end());
    }

    packet_t final_packet;
    final_packet.header() = stream_type_t::end_of_stream;
    packets.push_back(final_packet);

    return packets;
}

template <typename T, int MaxBodyLength>
T
reconstruct_chunk(const std::vector<stream_packet<MaxBodyLength>>& packets) {
    typedef stream_packet<MaxBodyLength> packet_t;

    std::stringstream sstr;
    for (const auto& packet : packets) {
        typename packet_t::length_t body_length = packet.body_length();
        sstr.write((const char*)packet.body(), body_length);
    }
    sstr.seekg(0);
    T v;
	{
	    cereal::BinaryInputArchive ar(sstr);
        ar(v);
	}

    return v;
}

} // magellan
