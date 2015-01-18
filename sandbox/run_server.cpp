#include <vector>
#include <iostream>

#include <magellan/magellan.hpp>

template <typename T>
class vector_session : public magellan::base_session {
	public:
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
			read(asio::buffer(length_), [&] (std::size_t l0) {
                if (l0 == 4) {
                    std::cout << "vector has size " << *length_ << "\n";
                    vec_.resize(*length_);
                    read(asio::buffer(vec_), [&] (std::size_t l1) {
                        if (l1 == *length_ * sizeof(T)) {
                            callback_(vec_);
                        }
                    });
                }
			});
		}

	protected:
		callback_t  callback_;
		uint32_t    length_[1];
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

