#ifndef MAGELLAN_SERVER_HPP_
#define MAGELLAN_SERVER_HPP_

#include <memory>

#include <asio.hpp>
#include "connection.hpp"

namespace magellan {


template <class Kernel>
class server {
    public:
        typedef std::shared_ptr<server>       ptr;
        typedef std::weak_ptr<server>         wptr;
        typedef std::shared_ptr<const server> const_ptr;
        typedef std::weak_ptr<const server>   const_wptr;

    public:
        template <typename... KernelArgs>
        server(asio::io_service& io_service, unsigned short port, KernelArgs&&... args);

        void handle_accept(const asio::error_code& e, connection::ptr_t conn);

    private:
        asio::ip::tcp::acceptor acceptor_;
        std::shared_ptr<Kernel> kernel_;

};

#include "server.ipp"


} // magellan


#endif /* MAGELLAN_SERVER_HPP_ */
