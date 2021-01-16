#include "HttpServer.h"
#include "HttpParser.h"
#include "HttpResponseBuilder.h"
#include "ChunkBuilder.h"
#include "ClientHandler.h"

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

HttpServer::HttpServer( const std::string& rootFolder ) : mRootFolder( rootFolder ), mSocket() {}

typedef void* ( *THREADFUNCPTR )( void* );
void HttpServer::start( uint32_t port )
{
  mSocket.start( port );

  while( true )
  {
    auto client = mSocket.nextConnection();
    fd          = &client;

    pthread_t thread;

    pthread_create( &thread, nullptr, (THREADFUNCPTR)&HttpServer::clientHandler, this );
  }
}

void* HttpServer::clientHandler( void* pTr )
{
  auto client = ClientHandler( *fd, mRootFolder );
  client.handle();
  pthread_exit(0);
}