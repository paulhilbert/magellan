#ifndef _PCL_COMPRESS_SENDER_HPP_
#define _PCL_COMPRESS_SENDER_HPP_

#include <deque>

#include "session.hpp"
#include "packet.hpp"

namespace pcl_compress {

class sender : virtual public session {
public:
    typedef std::shared_ptr<sender> ptr_t;
    typedef std::weak_ptr<sender> wptr_t;
    typedef std::shared_ptr<const sender> const_ptr_t;
    typedef std::weak_ptr<const sender> const_wptr_t;

public:
    sender(asio::io_context& io_context);
    virtual ~sender();

    void write(const stream_t& stream);

    virtual void on_finished_write() = 0;

protected:
    void do_write();

    void handle_write(const asio::error_code& error);

protected:
    stream_t current_stream_;
    std::deque<packet> write_queue_;
};

}  // pcl_compress

#endif /* _PCL_COMPRESS_SENDER_HPP_ */
