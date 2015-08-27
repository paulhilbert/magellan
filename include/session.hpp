#ifndef _MAGELLAN_SESSION_HPP_
#define _MAGELLAN_SESSION_HPP_

#include <chrono>

#include <asio.hpp>
#include <asio/spawn.hpp>
#include <asio/steady_timer.hpp>

#include "optional.hpp"
#include "stream_packet.hpp"

namespace magellan {

class session : public std::enable_shared_from_this<session> {
public:
    typedef std::shared_ptr<session> ptr_t;
    typedef std::weak_ptr<session> wptr_t;
    typedef std::shared_ptr<const session> const_ptr_t;
    typedef std::weak_ptr<const session> const_wptr_t;
    template <typename Period = std::ratio<1>>
    using duration = std::chrono::duration<int64_t, Period>;
    template <typename Period = std::ratio<1>>
    using opt_duration = std::optional<duration<Period>>;

    typedef ::asio::ip::tcp::socket socket_t;
    typedef ::asio::yield_context yield_context_t;

public:
    session(asio::ip::tcp::socket socket);

    asio::ip::tcp::socket& socket();

    const asio::ip::tcp::socket& socket() const;

    asio::io_context& context();

    template <typename Func, typename Period = std::ratio<1>>
    void async_do(Func&& f, opt_duration<Period> expiration = std::nullopt);

    template <typename Period = std::ratio<1>>
    void start(opt_duration<Period> expiration = std::nullopt);

    virtual std::optional<std::chrono::milliseconds> expiration() const;

protected:
    virtual void perform(asio::ip::tcp::socket&, asio::yield_context&);

    template <int MaxBodyLength, typename InputIterator>
    void send_stream(InputIterator first, InputIterator last, socket_t& s,
                     yield_context_t& yc);

    template <int MaxBodyLength, typename T>
    void send_chunk(T&& v, socket_t& s, yield_context_t& yc);

    template <int MaxBodyLength, typename OutputIterator>
    bool receive_stream(OutputIterator first, socket_t& s, yield_context_t& yc);

    template <int MaxBodyLength, typename T>
    std::optional<T> receive_chunk(socket_t& s, yield_context_t& yc);

    template <int MaxBodyLength>
    void send_packets_(const std::vector<stream_packet<MaxBodyLength>>& packets,
                       socket_t& s, yield_context_t& yc);

    template <typename Packet>
    std::optional<Packet> receive_packet_(socket_t& s, yield_context_t& yc);

protected:
    asio::ip::tcp::socket socket_;
    asio::io_context::strand strand_;
    asio::steady_timer timer_;
};

} // magellan

#include "session.ipp"

#endif /* _MAGELLAN_SESSION_HPP_ */
