#ifndef MAGELLAN_MONAD_HPP_
#define MAGELLAN_MONAD_HPP_

#include <memory>
#include <tuple>

namespace magellan {


template <class Derived>
struct monad {
    template <typename... Args>
    using state_t = std::shared_ptr<std::tuple<Args...>>;

    template <typename T>
    static state_t<T> ret(T&& value);

    template <typename T, typename... Args>
    static state_t<Args..., T> join(state_t<Args...> state, T&& value);

    template <typename... Args>
    static auto last_of(state_t<Args...> state) -> decltype(std::ref(std::get<std::tuple_size<std::tuple<Args...>>::value - 1>(*state)));

    template <typename Next, typename Func, typename... Args>
    void bind(state_t<Args...> state, Func&& func) {
        dynamic_cast<Derived*>(this)->fmap(state, [&, state] (Next&& next) { func(join(state, next)); });
    }
};

#include "monad.ipp"

} // magellan

#endif /* MAGELLAN_MONAD_HPP_ */
