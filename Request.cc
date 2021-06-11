#include "Request.h"

SERVER_NAMESPACE_BEGIN
template<typename SocketType>
server_map Request<SocketType>::ParserQuery()const{
  server_map result;
  if (query.empty())
	return result;

  std::size_t name_pos = 0;
  auto name_end_pos = std::string::npos;
  auto value_pos = std::string::npos;
  for (std::size_t c = 0; c < query.size(); ++c) {
	if (query[c] == '&') {
	  auto name = query.substr(
	  	name_pos,(name_end_pos == std::string::npos ? c : name_end_pos) - name_pos);
	  if (!name.empty()) {
		auto value = value_pos == std::string::npos ? std::string() :
			query.substr(value_pos,c - value_pos);
		result.emplace(std::move(name), percentage_converter::decode(value));
	  }
	  name_pos = c + 1;
	  name_end_pos = std::string::npos;
	  value_pos = std::string::npos;
	} else if (query[c] == '=') {
	  name_end_pos = c;
	  value_pos = c + 1;
	}
  }

  if (name_pos < query.size()) {
	auto name = query.substr(name_pos, name_end_pos - name_pos);
	if (!name.empty()) {
	  auto value = value_pos >= query.size() ? std::string() : query.substr(value_pos);
	  result.emplace(std::move(name), percentage_converter::decode(value));
	}
  }
  return result;
}
//}
SERVER_NAMESPACE_END