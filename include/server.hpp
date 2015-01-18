#ifndef MAGELLAN_SERVER_HPP_
#define MAGELLAN_SERVER_HPP_

#include <memory>
#include <asio.hpp>

namespace magellan {

template <class Session>
class server {
	public:
		typedef std::shared_ptr<server>        ptr_t;
		typedef std::weak_ptr<server>          wptr_t;
		typedef std::shared_ptr<const server>  const_ptr_t;
		typedef std::weak_ptr<const server>    const_wptr_t;

	public:
        template <typename... Args>
		static ptr_t create(asio::io_service& io_service, short port, Args&&... args);
		virtual ~server();

	protected:
		server(asio::io_service& io_service, short port);
        template <typename... Args>
		void  accept_connection(Args&&... args);

	protected:
        asio::ip::tcp::acceptor  acceptor_;
        asio::ip::tcp::socket    socket_;
};

template <class Session, typename... Args>
void run_server(short port, Args&&... args);

#include "server.ipp"

} // magellan

#endif /* MAGELLAN_SERVER_HPP_ */
