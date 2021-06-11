#ifndef MYSERVER_CONFIG_H
#define MYSERVER_CONFIG_H
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>

#define SERVER_NAMESPACE_BEGIN namespace MyServer{
#define SERVER_NAMESPACE_END }

SERVER_NAMESPACE_BEGIN
namespace asio = boost::asio;
typedef boost::system::error_code error_code;
typedef asio::streambuf SBUF;
typedef  std::unique_ptr<SBUF> U_PTR_SBUF;
typedef std::shared_ptr<SBUF> S_PTR_SBUF;
typedef std::list<std::pair<S_PTR_SBUF,std::function<void(const error_code&)>>> SEND_QUEUE_TYPE;
using SERVER_MAP = std::unordered_multimap<std::string, std::string>;

#ifdef _WIN32
#define strcasecmp _stricmp
#elif defined(UNIX) || defined(LINUX))
#include <string.h>
#define strcasecmp strcasecmp
#endif

template<typename SocketType>
asio::executor get_socket_executor(SocketType &socket) {
  return socket.get_executor();
}

#define GET_SOCKET_EXECUTOR(socket) socket.get_executor();

#define GET_SOCKET_REMOTEPOINT(socket) socket->lowest_layer().remote_endpoint();

#define WITH_SOCKET(x) x<SocketType>

#define TS_CAST(T,V) static_cast<T>(V);

class percentage_converter
{
 public:
  /// Returns percent-encoded string
  static std::string encode(const std::string& value) noexcept
  {
	static auto hex_chars = "0123456789ABCDEF";

	std::string result;
	result.reserve(value.size()); // Minimum size of result

	for (auto& chr : value)
	{
	  if (!((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||
		  chr == '-' || chr == '.' || chr == '_' || chr == '~'))
		result += std::string("%") + hex_chars[static_cast<unsigned char>(chr) >> 4] +
			hex_chars[static_cast<unsigned char>(chr) & 15];
	  else
		result += chr;
	}

	return result;
  }
  /// Returns percent-decoded string
  static std::string decode(const std::string& value) noexcept
  {
	std::string result;
	result.reserve(value.size() / 3 + (value.size() % 3)); // Minimum size of result

	for (std::size_t i = 0; i < value.size(); ++i)
	{
	  auto& chr = value[i];
	  if (chr == '%' && i + 2 < value.size())
	  {
		auto hex = value.substr(i + 1, 2);
		auto decoded_chr = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
		result += decoded_chr;
		i += 2;
	  }
	  else if (chr == '+')
		result += ' ';
	  else
		result += chr;
	}

	return result;
  }
};
SERVER_NAMESPACE_END
#endif //MYSERVER_CONFIG_H
