#ifndef PCL_COMPRESS_PACKET_RECEIVER_HPP_
#define PCL_COMPRESS_PACKET_RECEIVER_HPP_

#include <experimental/optional>
namespace stde = std::experimental;

#include "packet.hpp"

namespace pcl_compress {

class packet_receiver {
    public:
        typedef std::shared_ptr<packet_receiver>       ptr;
        typedef std::weak_ptr<packet_receiver>         wptr;
        typedef std::shared_ptr<const packet_receiver> const_ptr;
        typedef std::weak_ptr<const packet_receiver>   const_wptr;

    public:
        packet_receiver();

        virtual ~packet_receiver();

        stde::optional<chunk_t> insert(const packet& packet);

        bool end_of_stream() const;

        void reset();

    protected:
        chunk_t chunk_;
        bool    end_of_stream_;
};


} // pcl_compress

#endif /* PCL_COMPRESS_PACKET_RECEIVER_HPP_ */
