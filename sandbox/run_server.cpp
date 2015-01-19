#include <vector>
#include <iostream>

#include <magellan/magellan.hpp>

template <typename T>
struct receive_traits {
    static T init();
    static asio::mutable_buffers_1 buffer(T& value);
    static std::size_t request_length();
    template <typename... Args>
    static std::size_t request_length(magellan::monad<void>::state_t<Args...> prev_state);
};

template <>
uint32_t receive_traits<uint32_t>::init() {
    return uint32_t();
}

template <>
asio::mutable_buffers_1 receive_traits<uint32_t>::buffer(uint32_t& value) {
    return asio::buffer(&value, 4);
}

template <>
std::size_t receive_traits<uint32_t>::request_length() {
    return 4;
}

template <typename T>
struct receive_traits<std::vector<T>> {
    template <typename... Args>
    static std::vector<T> init(magellan::monad<void>::state_t<Args..., uint32_t> state);
    static asio::mutable_buffers_1 buffer(std::vector<T>& value);
    static std::size_t request_length();
    template <typename... Args>
    static std::size_t request_length(magellan::monad<void>::state_t<Args..., uint32_t> prev_state);
};

template <typename Scalar>
template <typename... Args>
std::vector<Scalar> receive_traits<std::vector<Scalar>>::init(magellan::monad<void>::state_t<Args..., uint32_t> state) {
    return std::vector<Scalar>(magellan::monad<void>::last_of(state));
}

template <typename Scalar>
asio::mutable_buffers_1 receive_traits<std::vector<Scalar>>::buffer(std::vector<Scalar>& value) {
    return asio::buffer(value);
}

template <typename Scalar>
template <typename... Args>
std::size_t receive_traits<std::vector<Scalar>>::request_length(magellan::monad<void>::state_t<Args..., uint32_t> prev_state) {
    return magellan::monad<void>::last_of(prev_state) * sizeof(Scalar);
}

template <typename T>
using last_type_t = typename std::tuple_element<std::tuple_size<T>::value - 1, T>::type;




template <typename T>
class vector_session : public magellan::base_session, public magellan::monad<vector_session<T>> {
    public:
        template <typename... Args>
        using state_t = typename magellan::monad<vector_session<T>>::template state_t<Args...>;
	public:
        typedef magellan::monad<vector_session<T>>     base_t;
		typedef std::shared_ptr<vector_session>        ptr_t;
		typedef std::weak_ptr<vector_session>          wptr_t;
		typedef std::shared_ptr<const vector_session>  const_ptr_t;
		typedef std::weak_ptr<const vector_session>    const_wptr_t;
		typedef std::vector<T>                         vec_t;
		typedef std::function<void (const vec_t&)>     callback_t;

	public:
		vector_session(asio::ip::tcp::socket socket, callback_t callback) :  magellan::base_session(std::move(socket)), callback_(std::move(callback)) {
		}

		virtual ~vector_session() {
		}

		void start() {
            std::cout << "start" << "\n";
            state_t<uint32_t> state0 = this->ret(receive_traits<uint32_t>::init()); //std::make_shared<std::tuple<uint32_t>>(std::make_tuple(receive_traits<uint32_t>::init()));
            std::cout << "before bind" << "\n";
            //magellan::monad<vector_session<T>>::template bla<std::vector<T>>(state, [&, state] (state_t<uint32_t, std::vector<T>>&& s) { callback_(this->last_of(s)); });
            //auto func = fmap<uint32_t, std::vector<T>>(state, [&, state] (state_t<uint32_t, std::vector<T>> s) { std::cout << std::get<0>(*s) << "\n"; [>callback_(std::get<1>(*s));<] });
            //fmap(state0,
                //[&, state0] (state_t<uint32_t> s0) {
                    //state_t<uint32_t, std::vector<T>> state1 = base_t::join(s0, receive_traits<std::vector<T>>::init(s0));
                    //fmap(state1,
                        //[&, state0, s0, state1] (state_t<uint32_t, std::vector<T>> s1) {
                            //for (const auto& v : std::get<1>(*s1)) {
                                //std::cout << v << "\n";
                            //}
                        //}
                    //)();
                //}
            //)();
            monad_bind<std::vector<T>>(
                state0,
                [&, state0] (state_t<uint32_t, std::vector<T>> state1) {
                    fmap(state1,
                        [&] (state_t<uint32_t, std::vector<T>> final_state) {
                            for (const auto& v : std::get<1>(*final_state)) {
                                std::cout << v << "\n";
                            }
                        }
                    )();
                }
            )();
		}

        template <typename Func, typename... Args>
        std::function<void (void)> fmap(state_t<Args...> state, Func func) {
            return [&, state, func] () {
                read(receive_traits<last_type_t<std::tuple<Args...>>>::buffer(base_t::last_of(state)), [&, state, func] (std::size_t len) {
                    func(state);
                });
            };
        }

        template <typename Next, typename... Args>
        static state_t<Args..., Next> fjoin(state_t<Args...> state) {
            return base_t::join(state, receive_traits<Next>::init(state));
        }

        template <typename Next, typename Func, typename... Args>
        std::function<void (void)> monad_bind(state_t<Args...> state, Func func) {
            return [&, state, func] () {
                fmap(state, [&, func] (state_t<Args...> s) {
                    state_t<Args..., Next> next = fjoin<Next, Args...>(s);
                    func(next);
                })();
            };
        }

	protected:
		callback_t  callback_;
		uint32_t    length_;
		vec_t       vec_;
};




int main(int argc, char const* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: run_server <port>" << "\n";
			return 1;
		}

		short  port = std::atoi(argv[1]);
		typedef vector_session<float>::callback_t  callback_t;

		magellan::run_server<vector_session<float>, callback_t>(port, [&] (const std::vector<float>& vec) { for (const auto& v : vec) { std::cout << v << "\n"; } });
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

