#ifndef _PCL_COMPRESS_SERVER_HPP_
#define _PCL_COMPRESS_SERVER_HPP_

#include <asio.hpp>
#include "server_session.hpp"
#include "session_pool.hpp"

namespace pcl_compress {

class server {
public:
    typedef std::shared_ptr<server> ptr_t;
    typedef std::weak_ptr<server> wptr_t;
    typedef std::shared_ptr<const server> const_ptr_t;
    typedef std::weak_ptr<const server> const_wptr_t;

public:
    server(asio::io_context& io_context,
           const asio::ip::tcp::endpoint& endpoint,
           compressed_cloud_t::ptr_t cloud);

    void start_accept();

    void handle_accept(server_session::ptr_t session, const asio::error_code& error);

protected:
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    session_pool::ptr_t pool_;
    compressed_cloud_t::ptr_t cloud_;
};

}  // pcl_compress

#endif /* _PCL_COMPRESS_SERVER_HPP_ */
