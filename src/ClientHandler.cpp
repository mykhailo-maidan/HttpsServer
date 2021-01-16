#include "ClientHandler.h"
#include "HttpParser.h"
#include "HttpResponseBuilder.h"
#include "ChunkBuilder.h"
#include "Logger.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <chrono>

constexpr auto SERVER_NAME        = "Test Http Server";
constexpr auto KEEP_ALIVE_TIMEOUT = 10;
constexpr auto KEEP_ALIVE_MAX_REQ = 5000;

namespace
{
Logger log( "ClientHandler" );
}
ClientHandler::ClientHandler( IOSocket socket, const std::string& rootFolder ) :
    mSocket( socket ), mRootFolder( rootFolder )
{}

void ClientHandler::handle()
{
  auto recv = readRequest();

  HttpParser parser( recv );
  auto       res = parser.parse();

  if( res.has_value() )
  {
    requestHandler( res.value() );
  }
  else
  {
    mSocket.write( HttpResponseBuilder()
                       .version( "HTTP/1.1" )
                       .code( HttpResponseBuilder::EResponseCode::eBadRequest )
                       .server( SERVER_NAME )
                       .connection( HttpResponseBuilder::EConnection::eClose ) );
  }

  mSocket.closeConnection();
}

std::string ClientHandler::readRequest()
{
  char        compare[] = "\r\n\r\n";
  std::string res;
  do
  {
    res += mSocket.read();
  } while( !std::includes( res.end() - 4, res.end(), compare, compare + 4 ) );

  return res;
}

void ClientHandler::requestHandler( const HttpParser::SHeader& request )
{
  switch( request.connection )
  {
  case HttpParser::EConnection::eKeepAlive:
  {
    keepAliveConnectionHandle( request );
    break;
  }
  default:
  {
    closeConnectionHandle( request );
    mSocket.closeConnection();
    break;
  }
  }
}

void ClientHandler::handleGetRequest( const HttpParser::SHeader& request, bool keepAlive )
{
  if( std::includes( request.url.begin(), request.url.end(), mRootFolder.begin(), mRootFolder.end() ) )
  {
    uploadResourse( request.url, keepAlive );
  }
  else
  {
    if( keepAlive )
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eNotFound )
                         .connection( HttpResponseBuilder::EConnection::eKeepAlive )
                         .keepAlive( KEEP_ALIVE_TIMEOUT, KEEP_ALIVE_MAX_REQ )
                         .server( SERVER_NAME ) );
    }
    else
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eNotFound )
                         .connection( HttpResponseBuilder::EConnection::eClose )
                         .server( SERVER_NAME ) );
    }
  }
}

void ClientHandler::uploadResourse( const std::string& url, bool keepAlive )
{
  std::ifstream file( url, std::ios_base::binary );
  if( file.is_open() )
  {
    constexpr auto SIZE = 1000000;

    char* buffer = new char[SIZE];

    if( keepAlive )
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eOK )
                         .connection( HttpResponseBuilder::EConnection::eKeepAlive )
                         .server( SERVER_NAME )
                         .keepAlive( KEEP_ALIVE_TIMEOUT, KEEP_ALIVE_MAX_REQ )
                         .transferEncoding( HttpResponseBuilder::ETransferEncoding::eChunked ) );
    }
    else
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eOK )
                         .connection( HttpResponseBuilder::EConnection::eClose )
                         .server( SERVER_NAME )
                         .transferEncoding( HttpResponseBuilder::ETransferEncoding::eChunked ) );
    }

    std::memset( buffer, 0, SIZE );
    ssize_t ret_in;
    do
    {
      file.read( buffer, SIZE );
      mSocket.write( ChunkBuilder( { buffer, file.gcount() } ) );
    } while( file );
    const char* end = "0\r\n\r\n";
    mSocket.write( { end, 5 } );
    delete[] buffer;
  }
  else
  {
    if( keepAlive )
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eNotFound )
                         .connection( HttpResponseBuilder::EConnection::eKeepAlive )
                         .server( SERVER_NAME )
                         .keepAlive( KEEP_ALIVE_TIMEOUT, KEEP_ALIVE_MAX_REQ ) );
    }
    else
    {
      mSocket.write( HttpResponseBuilder()
                         .version( "HTTP/1.1" )
                         .code( HttpResponseBuilder::EResponseCode::eNotFound )
                         .connection( HttpResponseBuilder::EConnection::eClose )
                         .server( SERVER_NAME ) );
    }
  }
}

void ClientHandler::closeConnectionHandle( const HttpParser::SHeader& request )
{
  switch( request.method )
  {
  case HttpParser::EMethod::eGet:
    handleGetRequest( request, false );
    break;

  default:
  {
    mSocket.write( HttpResponseBuilder()
                       .version( "HTTP/1.1" )
                       .code( HttpResponseBuilder::EResponseCode::eNotImplemented )
                       .connection( HttpResponseBuilder::EConnection::eClose )
                       .server( SERVER_NAME ) );
    break;
  }
  }
}

void ClientHandler::keepAliveConnectionHandle( const HttpParser::SHeader& request )
{
  handleRequestMethod( request );

  auto        t1       = std::chrono::high_resolution_clock::now();
  auto        t2       = std::chrono::high_resolution_clock::now();
  auto        duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  std::string recv;

  char compare[] = "\r\n\r\n";
  while( duration < KEEP_ALIVE_TIMEOUT )
  {
    recv += mSocket.read();
    if( recv.size() != 0 && std::includes( recv.end() - 4, recv.end(), compare, compare + 4 ) )
    {
      HttpParser parser( recv );
      auto       req = parser.parse();
      if( req.has_value() )
      {
        recv.clear();
        handleRequestMethod( req.value() );
      }
      else
      {
        mSocket.write( HttpResponseBuilder()
                           .version( "HTTP/1.1" )
                           .code( HttpResponseBuilder::EResponseCode::eBadRequest )
                           .server( SERVER_NAME )
                           .connection( HttpResponseBuilder::EConnection::eClose ) );
      }
    }

    t2       = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  }
}

void ClientHandler::handleRequestMethod( const HttpParser::SHeader& request )
{
  switch( request.method )
  {
  case HttpParser::EMethod::eGet:
  {
    handleGetRequest( request, true );
    break;
  }

  default:
  {
    mSocket.write( HttpResponseBuilder()
                       .version( "HTTP/1.1" )
                       .code( HttpResponseBuilder::EResponseCode::eNotImplemented )
                       .connection( HttpResponseBuilder::EConnection::eKeepAlive )
                       .keepAlive( KEEP_ALIVE_TIMEOUT, KEEP_ALIVE_MAX_REQ )
                       .server( SERVER_NAME ) );
    break;
  }
  }
}