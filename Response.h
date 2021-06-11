#ifndef MYSERVER_RESPONSE_H
#define MYSERVER_RESPONSE_H
#include <iostream>
#include <memory>
#include "common.h"

SERVER_NAMESPACE_BEGIN
class Session;
class Response : public std::enable_shared_from_this<Response>, std::ostream {
  friend class Server;
 public:
  Response(std::shared_ptr<Session> &_Session, long TimeOut) :
	  std::ostream(nullptr), session_(std::move(_Session)), timeout(TimeOut) {
	rdbuf(streambuf_.get());
  }

  std::size_t StreamSize() const {
	return streambuf_->size();
  }

 private:
  void WriteHeader(const SERVER_MAP&Header,std::size_t Size){
	bool content_length_written = false;
	bool chunked_transfer_encoding=false;
	for (const auto&e:Header)
	{
	  if (!content_length_written&&strcasecmp(e.first.c_str(),"content-length")==0)
	    content_length_written= true;
	  else if (!chunked_transfer_encoding&&strcasecmp(e.first.c_str(),"transfer-encoding")==0&&
	  _stricmp(e.second.c_str(),"chunked")==0)
	    chunked_transfer_encoding=true;
	  *this << e.first << ": " << e.second << "\r\n";
	}
	if(!content_length_written&&!chunked_transfer_encoding&&!close_connection_after_response)
	  *this << "Content-Length: " << Size << "\r\n\r\n";
	else
	  *this<<"\r\n";
  }

  U_PTR_SBUF streambuf_ = TS_CAST(U_PTR_SBUF, new SBUF());
  std::shared_ptr<Session> session_;
  long timeout;
  std::mutex send_queue_mutex;
  SEND_QUEUE_TYPE send_queue;
  bool close_connection_after_response = false;
};
SERVER_NAMESPACE_END

#endif //MYSERVER_RESPONSE_H