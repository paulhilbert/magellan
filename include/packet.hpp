#ifndef MAGELLAN_PACKET_HPP_
#define MAGELLAN_PACKET_HPP_

#include <memory>
#include "base_packet.hpp"

namespace magellan {

template <typename Header, int MaxBodyLength = 5000>
class packet : public base_packet<Header, MaxBodyLength> {
public:
    typedef std::shared_ptr<packet<Header, MaxBodyLength>>       ptr;
    typedef std::weak_ptr<packet<Header, MaxBodyLength>>         wptr;
    typedef std::shared_ptr<const packet<Header, MaxBodyLength>> const_ptr;
    typedef std::weak_ptr<const packet<Header, MaxBodyLength>>   const_wptr;
    typedef base_packet<Header, MaxBodyLength>                   base_t;
    using typename base_t::length_t;
    using typename base_t::data_t;

public:
    packet();

    packet(const packet& other);

    packet& operator=(const packet& other);

    virtual ~packet();

    const Header& header() const;

    Header& header();

    void encode();

    void decode();

protected:
    Header   header_;
};

template <int MaxBodyLength>
class packet<void, MaxBodyLength> : base_packet<void, MaxBodyLength> {
    public:
        typedef std::shared_ptr<packet<void, MaxBodyLength>>       ptr;
        typedef std::weak_ptr<packet<void, MaxBodyLength>>         wptr;
        typedef std::shared_ptr<const packet<void, MaxBodyLength>> const_ptr;
        typedef std::weak_ptr<const packet<void, MaxBodyLength>>   const_wptr;
        typedef base_packet<void, MaxBodyLength>                   base_t;
        using typename base_t::length_t;
        using typename base_t::data_t;

    public:
        packet();

        virtual ~packet();

        void encode();

        void decode();
};

template <int MaxBodyLength = 512>
using trivial_packet = packet<void, MaxBodyLength>;

}  // magellan

#include "packet.ipp"

#endif /* MAGELLAN_PACKET_HPP_ */
