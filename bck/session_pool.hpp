#ifndef _PCL_COMPRESS_SESSION_POOL_HPP_
#define _PCL_COMPRESS_SESSION_POOL_HPP_

#include "types.hpp"
#include <set>

namespace pcl_compress {

class session;

class session_pool {
public:
    typedef std::shared_ptr<session_pool> ptr_t;
    typedef std::weak_ptr<session_pool> wptr_t;
    typedef std::shared_ptr<const session_pool> const_ptr_t;
    typedef std::weak_ptr<const session_pool> const_wptr_t;

public:
    session_pool();

    virtual ~session_pool();

    void add(std::shared_ptr<session> s);

    void remove(std::shared_ptr<session> s);

protected:
    std::set<std::shared_ptr<session>> sessions_;
};

}  // pcl_compress

#endif /* _PCL_COMPRESS_SESSION_POOL_HPP_ */
