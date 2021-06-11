#ifndef MYSERVER_REQUEST_H_
#define MYSERVER_REQUEST_H_
#include "common.h"
#include "Connection.h"

SERVER_NAMESPACE_BEGIN
template<typename SocketType>
class Request {
  friend class Server;
  class Content final : public std::istream {
   public:
	Content(asio::streambuf &StreamBuf) :
		std::istream(&StreamBuf), streambuf_(StreamBuf) {}

	std::size_t Size() const {
	  return streambuf_.size();
	}

	[[nodiscard]] std::string String() const {
	  return std::string(streambuf_.data().begin(), streambuf_.data().end());
	}
   private:
	asio::streambuf &streambuf_;
  };

 public:
  Request(std::size_t MaxRequestSize, std::shared_ptr<WITH_SOCKET(Connection) > &_Connection) :
	  streambuf_(MaxRequestSize), connection_(_Connection), content(streambuf_) {}

  asio::ip::tcp::endpoint RemotePoint() const {
	try {
	  GET_SOCKET_REMOTEPOINT(connection_.lock()->socket)
	} catch (...) {}
	return asio::ip::tcp::endpoint();
  }

  SERVER_MAP ParserQuery()const;

  std::string method, path, query, http_version;
  Content content;
  SERVER_MAP header;
 private:
  std::weak_ptr<WITH_SOCKET(Connection) > connection_;
  asio::streambuf streambuf_;
};
SERVER_NAMESPACE_END

#endif //MYSERVER_REQUEST_H_
