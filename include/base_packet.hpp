#ifndef MAGELLAN_BASE_PACKET_HPP_
#define MAGELLAN_BASE_PACKET_HPP_

#include <memory>

namespace magellan {

template <typename Header, int MaxBodyLength = 512>
class base_packet {
public:
    typedef std::shared_ptr<base_packet<Header, MaxBodyLength>>       ptr;
    typedef std::weak_ptr<base_packet<Header, MaxBodyLength>>         wptr;
    typedef std::shared_ptr<const base_packet<Header, MaxBodyLength>> const_ptr;
    typedef std::weak_ptr<const base_packet<Header, MaxBodyLength>>   const_wptr;

    typedef char     data_t;
    typedef uint32_t length_t;

public:
    static constexpr uint32_t header_length =
        sizeof(length_t) + sizeof(Header);

public:
    base_packet();

    base_packet(const base_packet& other);

    base_packet& operator=(const base_packet& other);

    virtual ~base_packet();

    const data_t* data() const;

    data_t* data();

    length_t length() const;

    const data_t* body() const;

    data_t* body();

    length_t body_length() const;

    void set_body_length(length_t len);

protected:
    data_t   data_[header_length + MaxBodyLength];
    length_t body_length_;
};


}  // magellan

#include "base_packet.ipp"

#endif /* MAGELLAN_BASE_PACKET_HPP_ */
