/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <getopt.h>
#include "ServerSocket.h"
#include "Logger.h"
#include "HttpParser.h"
#include "HttpResponseBuilder.h"
#include "ChunkBuilder.h"
#include "HttpServer.h"

namespace
{
Logger log( "Main" );
}

int main( int argc, char **argv )
{


  int                  option_index{ 0 };
  static struct option long_options[] = { { "root-folder", required_argument, 0, 'r' },
                                          { "port", required_argument, 0, 'p' },
                                          { 0, 0, 0, 0 } };
  std::string          rootFolder;
  int                  port{ 0 };
  while( 1 )
  {
    auto retValue = getopt_long( argc, argv, "r:p", long_options, &option_index );
    if( retValue == -1 )
      break;

    switch( retValue )
    {
    case 'r':
    {
      rootFolder = optarg;
      break;
    }

    case 'p':
    {
      port = atoi( optarg );
      break;
    }

    default:
    {
      log.error( "Missed arguments\n --root-folder - root folder for server\n --port - server port" );
      exit(-1);
      break;
    }
    }
  }

  HttpServer server( "/" );
  server.start( 9000 );

  return 0;
}