#include <fstream>

namespace magellan {

template <typename Func, typename Period>
inline void
session::async_do(Func&& f, opt_duration<Period> expiration) {
    auto self(shared_from_this());
    asio::spawn(strand_, [this, self, f, expiration](yield_context_t yield) {
        try {
            if (expiration) {
                timer_.expires_from_now(*expiration);
            }
            f(std::ref(socket_), yield);
        } catch (std::exception& e) {
            socket_.close();
            timer_.cancel();
        }
    });

    if (expiration) {
        asio::spawn(strand_, [this, self](yield_context_t yield) {
            while (socket_.is_open()) {
                asio::error_code ignored_ec;
                timer_.async_wait(yield[ignored_ec]);
                if (timer_.expires_from_now() <= duration<Period>(0))
                    socket_.close();
            }
        });
    }
}

template <typename Period>
inline void
session::start(opt_duration<Period> expiration) {
    async_do([this](socket_t& s, yield_context_t yc) { perform(s, yc); },
             expiration);
}

template <int MaxBodyLength, typename InputIterator>
inline void
session::send_stream(InputIterator first, InputIterator last, socket_t& s,
                     yield_context_t yc) {
    std::vector<stream_packet<MaxBodyLength>> packets =
        make_stream_packets<MaxBodyLength>(first, last);
    send_packets_<MaxBodyLength>(packets, s, yc);
}

template <int MaxBodyLength, typename T>
inline void
session::send_chunk(T&& v, socket_t& s, yield_context_t yc) {
    std::vector<stream_packet<MaxBodyLength>> packets =
        make_chunk_packets<MaxBodyLength>(std::forward<T>(v));
    send_packets_<MaxBodyLength>(packets, s, yc);
}

template <int MaxBodyLength, typename OutputIterator>
inline bool
session::receive_stream(OutputIterator first, socket_t& s,
                        yield_context_t yc) {
    typedef stream_packet<MaxBodyLength> packet_t;
    typedef typename OutputIterator::value_type value_t;

    std::optional<value_t> v = receive_chunk<MaxBodyLength, value_t>(s, yc);
    while (v) {
        first++ = v;
    }
    std::optional<packet_t> eos_packet = receive_packet_<packet_t>(s, yc);
    if (!eos_packet || eos_packet->header() != stream_type_t::end_of_stream)
        return false;

    return true;
}

template <int MaxBodyLength, typename T>
inline std::optional<T>
session::receive_chunk(socket_t& s, yield_context_t yc) {
    typedef stream_packet<MaxBodyLength> packet_t;

    std::vector<packet_t> packets;
    auto current_packet = receive_packet_<packet_t>(s, yc);
    while (current_packet) {
        packets.push_back(*current_packet);
        if (current_packet->header() != stream_type_t::part_of_chunk) break;
        current_packet = receive_packet_<packet_t>(s, yc);
    }
    // if valid chunk at this point current_packet is != nullopt and has type
    // end_of_chunk
    if (!current_packet ||
        current_packet->header() != stream_type_t::end_of_chunk) {
        // did not receive all packets of chunk
        return std::nullopt;
    }

    // deserialize T from body data and return
    return reconstruct_chunk<T, MaxBodyLength>(packets);
}

template <int MaxBodyLength>
inline void
session::send_packets_(const std::vector<stream_packet<MaxBodyLength>>& packets,
                      socket_t& s, yield_context_t yc) {
    for (const auto& current_packet : packets) {
        asio::async_write(
            s, asio::buffer(current_packet.data(), current_packet.length()),
            yc);
    }
}

template <typename Packet>
inline std::optional<Packet>
session::receive_packet_(socket_t& s, yield_context_t yc) {
    Packet current_packet;
    asio::error_code error;
    // read header
    asio::async_read(
        s, asio::buffer(current_packet.data(), Packet::header_length),
        yc[error]);
    if (error) {
        return std::nullopt;
    }
    current_packet.decode();
    // read body
    asio::async_read(
        s, asio::buffer(current_packet.body(), current_packet.body_length()),
        yc[error]);
    if (error) return std::nullopt;
    return current_packet;
}

}  // magellan
