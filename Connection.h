#ifndef MYSERVER_CONNECTION_H
#define MYSERVER_CONNECTION_H

#include "common.h"

SERVER_NAMESPACE_BEGIN
template<typename SocketType>
class Connection : public std::enable_shared_from_this<Connection<SocketType>> {
 public:
  template<typename...Args>
  Connection(Args &&...args):socket(new SocketType(std::forward<Args>(args)...)) {
  }

  void Close() noexcept {
	error_code ec;
	socket->lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
	socket->lowest_layer().cancel(ec);
  }

  void SetTimeout(unsigned long seconds) {
	if (seconds == 0) {
	  timer = nullptr;
	  return;
	}
	timer = std::make_unique<asio::steady_timer>(
		get_socket_executor(*socket), std::chrono::seconds(seconds)
	);
	std::weak_ptr<Connection> self_weak(this->shared_from_this());
	timer->async_wait([self_weak](const error_code &ec) {
	  if (!ec) {
		if (auto self = self_weak.lock())
		  self->close();
	  }
	});
  }

  void CancelTimeout() {
	try { timer->cancel(); }
	catch (...) {}
  }

 private:
  std::unique_ptr<SocketType> socket;
  std::unique_ptr<asio::steady_timer> timer;
};
SERVER_NAMESPACE_END

#endif //MYSERVER_CONNECTION_H
