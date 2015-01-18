template <class Derived>
template <typename T>
inline typename monad<Derived>::template state_t<T> monad<Derived>::monad::ret(T&& value) {
    return state_t<T>(new std::tuple<T>(std::forward<T>(value)));
}

template <class Derived>
template <typename T, typename... Args>
inline typename monad<Derived>::template state_t<Args..., T> monad<Derived>::monad::join(state_t<Args...> state, T&& value) {
    return std::make_shared<std::tuple<Args..., T>>(std::tuple_cat(*state, *ret(value)));
}

template <class Derived>
template <typename... Args>
inline auto monad<Derived>::monad::last_of(state_t<Args...> state) -> decltype(std::ref(std::get<std::tuple_size<std::tuple<Args...>>::value - 1>(*state))) {
    return std::ref(std::get<std::tuple_size<std::tuple<Args...>>::value - 1>(*state));
}
