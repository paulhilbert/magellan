#ifndef _PCL_COMPRESS_RECEIVER_HPP_
#define _PCL_COMPRESS_RECEIVER_HPP_

#include "session.hpp"
#include "packet.hpp"
#include "packet_receiver.hpp"

namespace pcl_compress {

class receiver : virtual public session {
    public:
        typedef std::shared_ptr<receiver>       ptr_t;
        typedef std::weak_ptr<receiver>         wptr_t;
        typedef std::shared_ptr<const receiver> const_ptr_t;
        typedef std::weak_ptr<const receiver>   const_wptr_t;

    public:
        receiver(asio::io_context& io_context, session_pool::ptr_t pool);

        virtual ~receiver();


    protected:
        void read(const asio::error_code& error);

        void handle_read_header(const asio::error_code& error);

        void handle_read_body(const asio::error_code& error);

        virtual void on_chunk_ready(const chunk_t& chunk) = 0;

        virtual void on_stream_ready() = 0;

    protected:
        packet             read_packet_;
        packet_receiver    receiver_;
};


} // pcl_compress

#endif /* _PCL_COMPRESS_RECEIVER_HPP_ */
