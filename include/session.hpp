#ifndef _MAGELLAN_SESSION_HPP_
#define _MAGELLAN_SESSION_HPP_

#include <asio.hpp>
#include <asio/spawn.hpp>

//#include "session_pool.hpp"

namespace magellan {

class session : public std::enable_shared_from_this<session> {
public:
    typedef std::shared_ptr<session> ptr_t;
    typedef std::weak_ptr<session> wptr_t;
    typedef std::shared_ptr<const session> const_ptr_t;
    typedef std::weak_ptr<const session> const_wptr_t;

public:
    session(asio::ip::tcp::socket socket);

    asio::ip::tcp::socket& socket();

    const asio::ip::tcp::socket& socket() const;

    asio::io_context& context();

    template <typename Func>
    void async_do(Func&& f);

    void start();

protected:
    virtual void perform(asio::ip::tcp::socket&, asio::yield_context&);

protected:
    asio::ip::tcp::socket socket_;
    asio::io_context::strand strand_;
};

} // magellan

#include "session.ipp"

#endif /* _MAGELLAN_SESSION_HPP_ */
