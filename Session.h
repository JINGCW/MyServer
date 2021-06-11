#ifndef MYSERVER_SESSION_H
#define MYSERVER_SESSION_H
#include "Connection.h"
#include "Request.h"
#include "common.h"

SERVER_NAMESPACE_BEGIN
template<typename SocketType>
class Session{
 public:
  Session(std::size_t MaxRequestSize,std::shared_ptr<WITH_SOCKET(Connection)>&_Connection):
//  connection_(std::move(_connection)){
  connection_(_Connection){
	request_(new WITH_SOCKET(Request)(MaxRequestSize, connection_));
  }
 private:
  std::shared_ptr<Connection<SocketType>> connection_;
  std::shared_ptr<Request<SocketType>> request_;
};
SERVER_NAMESPACE_END

#endif //MYSERVER_SESSION_H