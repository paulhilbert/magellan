#ifndef MAGELLAN_MONAD_HPP_
#define MAGELLAN_MONAD_HPP_

#include <utility>

namespace magellan {


template <template <typename...> class M, typename... Ts>
struct monad {
    static auto mreturn(Ts&&... args) -> M<Ts...>;

    template <typename F>
    static auto mbind(M<Ts...>&& m, F f) -> decltype(f(std::declval<Ts>()...));

    //static auto mfail(std::string msg) -> M<Ts...>;
};

template <template <typename...> class M, typename... Ts>
auto mreturn(Ts&&... args) -> M<Ts...>;

template <template <typename...> class M, typename F, typename... Ts>
auto mbind(M<Ts...>&& m, F f) -> decltype(f(std::declval<Ts>()...));

template <template <typename...> class M, typename F, typename... Ts>
auto mbind(M<std::tuple<Ts...>>&& m, F f) -> decltype(f(std::declval<Ts>()...));

//template <template <typename...> class M, typename... Ts>
//auto mfail(std::string msg) -> M<Ts...>;

template <template <typename...> class M, typename... Ts>
class monadic_chain {
    public:
        typedef M<Ts...> monad_t;

        template <typename M1>
        struct traits;

    public:
        monadic_chain(Ts&&... args);

        monadic_chain(M<Ts...>&& m);

        template <typename F>
        auto operator()(F f);

        monad_t operator()();

        template <typename F, typename... Fs>
        auto chain(F f, Fs... fs);

        auto chain();

    protected:
        monadic_chain(std::tuple<Ts...>&& args);

    protected:
        monad_t monad_;
};

template <template <typename...> class M, typename... Ts>
monadic_chain<M, Ts...> make_chain(Ts&&... args);

template <template <typename...> class M, typename... Ts>
monadic_chain<M, Ts...> make_chain(M<Ts...>&& m);

template <template <typename...> class M, typename... Ts, typename... Fs>
auto chain(M<Ts...>&& m, Fs... fs);


#include "monad.ipp"

} // magellan

#endif /* MAGELLAN_MONAD_HPP_ */
