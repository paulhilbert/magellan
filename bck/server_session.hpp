#ifndef _PCL_COMPRESS_SERVER_SESSION_HPP_
#define _PCL_COMPRESS_SERVER_SESSION_HPP_

#include <deque>
#include <asio.hpp>
#include "sender.hpp"

namespace pcl_compress {

class server_session : public sender {
public:
    typedef std::shared_ptr<server_session> ptr_t;
    typedef std::weak_ptr<server_session> wptr_t;
    typedef std::shared_ptr<const server_session> const_ptr_t;
    typedef std::weak_ptr<const server_session> const_wptr_t;

public:
    server_session(asio::io_context& io_context, session_pool::ptr_t pool);

    void start(compressed_cloud_t::ptr_t cloud);

    void on_finished_write();

protected:
    bool write_global_;

protected:
    compressed_cloud_t::ptr_t cloud_;
};

} // pcl_compress

#endif /* _PCL_COMPRESS_SERVER_SESSION_HPP_ */
