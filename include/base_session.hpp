#ifndef MAGELLAN_BASE_SESSION_HPP_
#define MAGELLAN_BASE_SESSION_HPP_

#include <memory>
#include <asio.hpp>

namespace magellan {

class base_session : public std::enable_shared_from_this<base_session> {
    public:
        typedef std::shared_ptr<base_session>       ptr;
        typedef std::weak_ptr<base_session>         wptr;
        typedef std::shared_ptr<const base_session> const_ptr;
        typedef std::weak_ptr<const base_session>   const_wptr;

    public:
        base_session(asio::ip::tcp::socket socket);
        virtual ~base_session();

        virtual void start() = 0;

    protected:
        template <typename Buffer, typename Func>
        void read(Buffer&& buffer, Func&& handler);

        template <typename Buffer, typename Func>
        void write(Buffer&& buffer, Func&& handler);

    protected:
        asio::ip::tcp::socket  socket_;
};

#include "base_session.ipp"


} // magellan

#endif /* MAGELLAN_BASE_SESSION_HPP_ */
