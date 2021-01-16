#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#include <stdint.h>
#include "IOSocket.h"

class ServerSocket
{
public:
  explicit ServerSocket()  = default;
  ~ServerSocket() = default;

  void start( uint32_t portNumber );

  IOSocket nextConnection();

private:
  int32_t createSocket( uint32_t port );

private:
  int32_t mServerDescriptor{ -1 };
};

#endif //_SERVER_SOCKET_H_
