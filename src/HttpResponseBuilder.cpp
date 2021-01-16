#include <time.h>

#include "HttpResponseBuilder.h"
#include "Logger.h"

namespace
{
Logger log( "HttpResposeBuilder" );
}

constexpr auto END_OF_LINE = "\r\n";

Self& HttpResponseBuilder::version( const std::string& version )
{
  if( mResponse.size() == 0 )
  {
    mResponse += version + ' ';
  }
  else
  {
    log.warning( "Http version already setted. Check code." );
  }

  return *this;
}

Self& HttpResponseBuilder::code( EResponseCode code )
{
  switch( code )
  {
  case EResponseCode::eOK:
  {
    mResponse.append( "200 OK" );
    break;
  }
  case EResponseCode::eBadRequest:
  {
    mResponse.append( "400 Bad Request" );
    break;
  }
  case EResponseCode::eNotFound:
  {
    mResponse.append( "404 Not Found" );
    break;
  }
  default:
  {
    mResponse.append( "501 Not Implemented" );
    break;
  }
  }
  mResponse.append( END_OF_LINE );

  return *this;
}

Self& HttpResponseBuilder::connection( EConnection connection )
{
  if( mConnectionType.size() == 0 )
  {
    mConnectionType.append( "Connection: " );
    mConnectionType.append( connection == EConnection::eKeepAlive ? "keep-alive" : "close" );
    mConnectionType.append( END_OF_LINE );
  }
  else
  {
    log.warning( "Connection type already setted. Check code." );
  }

  return *this;
}

Self& HttpResponseBuilder::server( const std::string& server )
{
  if( mServerName.size() == 0 )
  {
    mServerName.append( "Server: " );
    mServerName.append( server );
    mServerName.append( END_OF_LINE );
  }
  else
  {
    log.warning( "Server Name already setted. Check code." );
  }

  return *this;
}

Self& HttpResponseBuilder::keepAlive( uint32_t timeout, uint32_t max )
{
  if( mKeepAlive.size() == 0 )
  {
    mKeepAlive.append( "Keep-Alive: " );
    mKeepAlive.append( "timeout=" + std::to_string( timeout ) + ' ' );
    mKeepAlive.append( "max=" + std::to_string( max ) );
    mKeepAlive.append( END_OF_LINE );
  }
  else
  {
    log.warning( "Keep Alive already setted. Check code." );
  }

  return *this;
}

Self& HttpResponseBuilder::transferEncoding( ETransferEncoding encoding )
{
  if( mTransferEncoding.size() != 0 )
  {
    log.warning( "Transfer Encoding already setted." );
    return *this;
  }
  switch( encoding )
  {
  case ETransferEncoding::eChunked:
  {
    mTransferEncoding.append( "Transfer-Encoding: chunked" );
    mTransferEncoding.append( END_OF_LINE );
    break;
  }
  default:
  {
    log.warning( "Wrong type of encoding" );
    break;
  }
  }
  return *this;
}

HttpResponseBuilder::operator std::string()
{
  mResponse.append( mConnectionType );
  mResponse.append( mServerName );
  mResponse.append( getData() );
  mResponse.append( mKeepAlive );
  mResponse.append( mTransferEncoding );
  mResponse.append( END_OF_LINE );
  return std::move( mResponse );
}

std::string HttpResponseBuilder::getData()
{
  time_t     rawtime;
  struct tm* ptm;

  time( &rawtime );

  ptm = gmtime( &rawtime );
  char buffer[100]{ 0 };
  strftime( buffer, 80, "%a, %d %b %Y %T GTM", ptm );

  std::string data;
  data.append( "Data: " );
  data.append( buffer );
  data.append( END_OF_LINE );

  return std::move( data );
}