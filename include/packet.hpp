#ifndef MAGELLAN_PACKET_HPP_
#define MAGELLAN_PACKET_HPP_

#include "types.hpp"

namespace magellan {

class packet {
public:
    typedef std::shared_ptr<packet> ptr;
    typedef std::weak_ptr<packet> wptr;
    typedef std::shared_ptr<const packet> const_ptr;
    typedef std::weak_ptr<const packet> const_wptr;
    typedef char data_t;
    typedef uint32_t length_t;

    typedef enum class type_ : data_t {
        part_of_chunk,
        end_of_chunk,
        end_of_stream
    } type_t;

public:
    static constexpr uint32_t header_type_length = sizeof(data_t);
    static constexpr uint32_t header_size_length = sizeof(length_t);
    static constexpr uint32_t header_length =
        header_type_length + header_size_length;
    static constexpr uint32_t max_body_length = 256;

public:
    packet();

    packet(const packet& other);

    packet& operator=(const packet& other);

    virtual ~packet();

    const data_t* data() const;

    data_t* data();

    type_t packet_type() const;

    void set_packet_type(type_t packet_type);

    length_t length() const;

    const data_t* body() const;

    data_t* body();

    length_t body_length() const;

    void set_body_length(length_t len);

    bool decode_header();

    void encode_header();

    static std::vector<packet> make_packets(const chunk_t& chunk);

    static std::vector<packet> make_packets(const stream_t& stream);

protected:
    data_t   data_[header_length + max_body_length];
    length_t body_length_;
    type_t   type_;
};


}  // magellan

#endif /* MAGELLAN_PACKET_HPP_ */
