#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "ServerSocket.h"
#include "IOSocket.h"
#include "ClientHandler.h"

#include <string>
#include <pthread.h>
#include <map>



class HttpServer
{
public:
  HttpServer( const std::string& rootFolder );

  ~HttpServer() = default;

  void start( uint32_t port );

private:
  void* clientHandler( void* pTr );

private:
  std::string                        mRootFolder;
  ServerSocket                       mSocket;
  std::map<pthread_t, ClientHandler> mThreadList;
  IOSocket *fd;
};

#endif // _HTTP_SERVER_H_