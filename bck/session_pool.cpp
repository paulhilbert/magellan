#include <session_pool.hpp>
#include <session.hpp>

namespace pcl_compress {

session_pool::session_pool() {
}

session_pool::~session_pool() {
}

void
session_pool::add(session::ptr_t s) {
    sessions_.insert(s);
}

void
session_pool::remove(session::ptr_t s) {
    sessions_.erase(s);
}

}  // pcl_compress
