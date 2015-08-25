template <typename T>
inline auto
monad<std::future, T>::mreturn(T&& arg) -> std::future<T> {
    std::promise<T> p;
    p.set_value(std::forward<T>(arg));
    return p.get_future();
}

template <typename T>
template <typename F>
inline auto
monad<std::future, T>::mbind(std::future<T>&& fut, F f) -> decltype(f(std::declval<T>())) {
    return f(fut.get());
}
