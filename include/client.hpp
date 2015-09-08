#ifndef MAGELLAN_CLIENT_HPP_
#define MAGELLAN_CLIENT_HPP_

#include "session.hpp"

namespace magellan {

class client {
    public:
        typedef std::shared_ptr<client>       ptr;
        typedef std::weak_ptr<client>         wptr;
        typedef std::shared_ptr<const client> const_ptr;
        typedef std::weak_ptr<const client>   const_wptr;

    public:
        client();

        virtual ~client();

        template <typename Session, typename... Args>
        void connect(std::string host, short port, Args&&... args);

        void run();

    protected:
        asio::io_context io_context_;
};

} // magellan

#include "client.ipp"

#endif /* MAGELLAN_CLIENT_HPP_ */
