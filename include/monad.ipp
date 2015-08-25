template <template <typename...> class M, typename... Ts>
inline auto
mreturn(Ts&&... args) -> M<Ts...> {
    return monad<M, Ts...>::mreturn(std::forward<Ts>(args)...);
}

template <template <typename...> class M, typename F, typename... Ts>
inline auto
mbind(M<Ts...>&& m, F f) -> decltype(f(std::declval<Ts>()...)) {
    return monad<M, Ts...>::template mbind<F>(std::move(m), f);
}

template <template <typename...> class M, typename F, std::size_t... Is, typename... Ts>
inline auto
mbind_tuple(M<std::tuple<Ts...>>&& m, F f, std::index_sequence<Is...>) -> decltype(f(std::declval<Ts>()...)) {
    std::cout << "tuple call" << "\n";
    return monad<M, Ts...>::template mbind<F>(std::move(m), [&] (std::tuple<Ts...> args) { return f(std::get<Is>(args)...); });
}

template <template <typename...> class M, typename F, typename... Ts>
inline auto
mbind(M<std::tuple<Ts...>>&& m, F f) -> decltype(f(std::declval<Ts>()...)) {
    return mbind_tuple(std::move(m), f, std::index_sequence_for<Ts...>{});
}

//template <template <typename...> class M, typename... Ts>
//inline auto mfail(std::string msg) -> M<Ts...> {
    //return monad<M, Ts...>::fail(msg);
//}

template <template <typename...> class M, typename... Ts>
template <template <typename...> class M2, typename... Xs>
struct monadic_chain<M, Ts...>::traits<M2<Xs...>> {
    typedef monadic_chain<M2, Xs...> type_t;
};

template <template <typename...> class M, typename... Ts>
inline
monadic_chain<M, Ts...>::monadic_chain(Ts&&... args) : monad_(mreturn<M, Ts...>(std::forward<Ts>(args)...)) {
}

template <template <typename...> class M, typename... Ts>
inline
monadic_chain<M, Ts...>::monadic_chain(M<Ts...>&& m) : monad_(std::move(m)) {
}

template <template <typename...> class M, typename... Ts>
template <typename F>
inline auto
monadic_chain<M, Ts...>::operator()(F f) {
    typedef typename traits<decltype(f(std::declval<Ts>()...))>::type_t chain_t;
    return chain_t(std::move(mbind(std::move(monad_), f)));
}

template <template <typename...> class M, typename... Ts>
inline typename monadic_chain<M, Ts...>::monad_t
monadic_chain<M, Ts...>::operator()() {
    return std::move(monad_);
}

template <template <typename...> class M, typename... Ts>
template <typename F, typename... Fs>
inline auto
monadic_chain<M, Ts...>::chain(F f, Fs... fs) {
    return this->operator()(f).chain(fs...);
}

template <template <typename...> class M, typename... Ts>
inline auto
monadic_chain<M, Ts...>::chain() {
    return this->operator()();
}

template <template <typename...> class M, typename... Ts>
inline
monadic_chain<M, Ts...>::monadic_chain(std::tuple<Ts...>&& args) : monad_(mreturn(std::forward<Ts>(args)...)) {
}

template <template <typename...> class M, typename... Ts>
inline monadic_chain<M, Ts...>
make_chain(Ts&&... args) {
    return monadic_chain<M, Ts...>(std::forward<Ts>(args)...);
}

template <template <typename...> class M, typename... Ts>
inline monadic_chain<M, Ts...>
make_chain(M<Ts...>&& m) {
    return monadic_chain<M, Ts...>(std::move(m));
}

template <template <typename...> class M, typename... Ts, typename... Fs>
inline auto
chain(M<Ts...>&& m, Fs... fs) {
    return make_chain<M, Ts...>(std::move(m)).chain(fs...);
}
