#ifndef MAGELLAN_FUTURE_MONAD_HPP_
#define MAGELLAN_FUTURE_MONAD_HPP_

#include <future>

#include "monad.hpp"

namespace magellan {


template <typename T>
struct monad<std::future, T> {
    static auto mreturn(T&& arg) -> std::future<T>;

    template <typename F>
    static auto mbind(std::future<T>&& fut, F f) -> decltype(f(std::declval<T>()));

    //static auto mfail(std::string msg) -> M<Ts...>;
};

#include "future_monad.ipp"

//template <typename T>
//std::shared_future<T> mreturn(const T& t) {
    //std::cout << "mreturn" << "\n";
    //return std::async(std::launch::deferred, [=] () { std::cout << "return func" << "\n"; return t; }).share();
//}

//std::shared_future<void> mreturn() {
    //return std::async(std::launch::deferred, [=] () { }).share();
//}

//template <typename T>
//std::shared_future<T> mjoin(const std::shared_future<std::shared_future<T>>& f) {
    //std::cout << "mjoin" << "\n";
    //return std::async(std::launch::async, [=] () { std::cout << "mjoin func" << "\n"; return f.get().get(); }).share();
//}

//template <typename T, typename F>
//auto operator>>=(const std::shared_future<T>& m, F&& f) -> decltype(f(std::declval<T>())) {
    //std::cout << "bind" << "\n";
    //return mjoin(std::async(std::launch::async, [=] () { std::cout << "bind func" << "\n"; return f(m.get()); }).share());
//}

//#define DO(var,monad,body)          \
//((monad) >>= [&](auto var) { \
    //return body;                            \
//})


} // magellan

#endif /* MAGELLAN_FUTURE_MONAD_HPP_ */
