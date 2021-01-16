#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "IOSocket.h"
#include "Logger.h"

namespace
{
Logger log( "IOSocket" );
};

IOSocket::IOSocket( int32_t ioDescriptor ) : mIODescriptor( ioDescriptor )
{
  //fcntl( mIODescriptor, F_SETFL, O_NONBLOCK );
}

std::string IOSocket::read()
{
  char buff[10000] = { 0 };
  auto bytes       = ::recv( mIODescriptor, buff, sizeof( buff ), MSG_DONTWAIT );
  if( bytes <= 0 )
  {
    return "";
  }

  buff[bytes] = '\0';

  return std::string( buff );
}

void IOSocket::write( const std::string& message )
{
  auto size  = message.size();
  auto count = ::send( mIODescriptor, message.c_str(), message.size(), MSG_NOSIGNAL );

  if( count == -1 )
  {
    log.error( "Error in write data to file descriptor:%d", mIODescriptor );
  }
  fsync( mIODescriptor );
}

void IOSocket::closeConnection()
{
  if( close( mIODescriptor ) < 0 )
  {
    log.warning( "Cannot close socket with number %d", mIODescriptor );
    return;
  }

  log.debug( "Connection closed. File Descriptor:%d", mIODescriptor );
}
