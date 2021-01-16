#ifndef _IO_SOCKET_H_
#define _IO_SOCKET_H_

#include <string>

class IOSocket
{
public:
  IOSocket( int32_t ioDescriptor );

  std::string read();

  void write( const std::string& message );

  void closeConnection();

private:
  int32_t mIODescriptor;
};

#endif // _IO_SOCKET_H_