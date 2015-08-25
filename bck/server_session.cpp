#include <server_session.hpp>

using asio::ip::tcp;

namespace pcl_compress {

server_session::server_session(asio::io_context& io_context, session_pool::ptr_t pool)
    : session(io_context, pool), sender(io_context, pool), write_global_(true) {
}

void
server_session::start(compressed_cloud_t::ptr_t cloud) {
    cloud_ = cloud;

    stream_t stream;
    if (write_global_) {
        stream = {cloud->global_data};
    } else {
        stream = cloud->patch_image_data;
    }
    write(stream);
}

void server_session::on_finished_write() {
    write_global_ = !write_global_;
    if (!write_global_) start(cloud_);
}

} // pcl_compress
