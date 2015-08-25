#ifndef _MAGELLAN_TYPES_HPP_
#define _MAGELLAN_TYPES_HPP_

#include <vector>
#include <memory>

namespace magellan {

typedef std::vector<uint8_t> chunk_t;
typedef std::vector<chunk_t> stream_t;

typedef struct compressed_cloud_ {
    typedef std::shared_ptr<compressed_cloud_> ptr_t;
    typedef std::shared_ptr<const compressed_cloud_> const_ptr_t;

    chunk_t global_data;
    std::vector<std::vector<uint8_t>> patch_image_data;

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(global_data);
        ar(patch_image_data);
    }
} compressed_cloud_t;

} // magellan

#endif /* _MAGELLAN_TYPES_HPP_ */
