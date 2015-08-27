#ifndef _MAGELLAN_SESSION_HPP_
#define _MAGELLAN_SESSION_HPP_

#include <chrono>

#include <asio.hpp>
#include <asio/spawn.hpp>
#include <asio/steady_timer.hpp>

#include "optional.hpp"

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

protected:
    asio::ip::tcp::socket socket_;
    asio::io_context::strand strand_;
    asio::steady_timer timer_;
};

} // magellan

#include "session.ipp"

#endif /* _MAGELLAN_SESSION_HPP_ */
