#ifndef MAGELLAN_FUTURE_MONAD_HPP_
#define MAGELLAN_FUTURE_MONAD_HPP_

#include <future>


namespace magellan {


template <typename T>
std::shared_future<T> mreturn(const T& t) {
    return std::async(std::launch::async, [=] () { return t; }).share();
}

std::shared_future<void> mreturn() {
    return std::async(std::launch::async, [=] () { }).share();
}

template <typename T>
std::shared_future<T> mjoin(const std::shared_future<std::shared_future<T>>& f) {
    return std::async(std::launch::async, [=] () { return f.get().get(); }).share();
}

template <typename T, typename F>
auto operator>>=(const std::shared_future<T>& m, F&& f) -> decltype(f(std::declval<T>())) {
    return mjoin(std::async(std::launch::async, [=] () { return f(m.get()); }).share());
}

#define DO(var,monad,body)          \
((monad) >>= [&](auto var) { \
    return body;                            \
})


} // magellan

#endif /* MAGELLAN_FUTURE_MONAD_HPP_ */
