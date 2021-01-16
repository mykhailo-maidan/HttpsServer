#include "ServerSocket.h"
#include "IOSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <cstring>

#include "IOSocket.h"
#include "Logger.h"


namespace 
{
  Logger log("ServerSocket");
}
void ServerSocket::start( uint32_t portNumber )
{
  mServerDescriptor = createSocket( portNumber );
}

IOSocket ServerSocket::nextConnection()
{
  struct sockaddr_in clientname;
  socklen_t size = sizeof(clientname);
  auto client = accept( mServerDescriptor, (struct sockaddr *) &clientname, &size );
  log.info("Client connected:%s",inet_ntoa(clientname.sin_addr));
  
  return IOSocket( client );
}
int32_t ServerSocket::createSocket( uint32_t port )
{
  int                 on = 1;
  struct sockaddr_in6 addr;

  if( ( mServerDescriptor = socket( AF_INET6, SOCK_STREAM, 0 ) ) < 0 )
  {
    log.error( "Process create socket failed" );
    return -1;
  }
  if( setsockopt( mServerDescriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof( on ) ) < 0 )
  {
    log.error( "Function setsockopt(SO_REUSEADDR) failed" );
    return -1;
  }

  memset( &addr, 0, sizeof( addr ) );
  addr.sin6_family = AF_INET6;
  addr.sin6_port   = htons( port );
  addr.sin6_addr   = in6addr_any;

  if( bind( mServerDescriptor, (struct sockaddr *)&addr, sizeof( addr ) ) < 0 )
  {
    log.error( "Can't bind port %d", port );
    return -1;
  }

  if( listen( mServerDescriptor, 10 ) != 0 )
  {
    log.error( "Can't configure listening port" );
    return -1;
  }

  return mServerDescriptor;
}
