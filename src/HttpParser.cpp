#include "HttpParser.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

HttpParser::HttpParser( const std::string& data ) : mDataForParsing( data ) {}

HttpParser::~HttpParser() {}

std::optional<HttpParser::SHeader> HttpParser::parse()
{
  std::string       data( mDataForParsing );
  std::stringstream io_str( data );

  std::vector<std::string> tokens;

  std::string token;
  char        delim = '\n';
  while( std::getline( io_str, token, delim ) )
  {
    tokens.emplace_back( token );
  }

  if( tokens.size() < 4 )
  {
    return std::nullopt;
  }

  SHeader header;

  parseMethod( tokens[0], header );

  tokens.erase( tokens.begin() );

  auto fieldCount{0};

  for( auto& it: tokens )
  {
    if( it.find( "User-Agent:" ) != std::string::npos )
    {
      header.user_agent.append( it.begin() + it.find_last_of( ':' ) + 2, it.end() - 1 );
      ++fieldCount;
    }
    else if( it.find( "Accept:" ) != std::string::npos )
    {
      header.accept.append( it.begin() + it.find_last_of( ':' ) + 2, it.end() - 1 );
      ++fieldCount;
    }
    else if( it.find( "Host:" ) != std::string::npos )
    {
      header.host.append( it.begin() + it.find_first_of( ':' ) + 2, it.end() - 1 );
      ++fieldCount;
    }
    else if( it.find( "Connection:" ) != std::string::npos )
    {
      header.connection = parseConnection( it );
      ++fieldCount;
    }
  }

  if( fieldCount < 4)
  {
    return std::nullopt;
  }
  return header;
}

void HttpParser::parseMethod( const std::string& token, SHeader& header )
{
  std::stringstream io( token );

  char                     delim = ' ';
  std::string              t;
  std::vector<std::string> method;
  while( std::getline( io, t, delim ) )
  {
    method.emplace_back( t );
  }

  if( method[0].compare( "GET" ) == 0 )
  {
    header.method = EMethod::eGet;
  }
  else
  {
    header.method = EMethod::eNone;
  }

  header.url     = method[1];
  header.version = { method[2].begin(), method[2].end() - 1 };
}

HttpParser::EConnection HttpParser::parseConnection( const std::string& data )
{
  std::string temp = data;
  temp.erase( temp.begin(), temp.begin() + temp.find_last_of( ':' ) + 2 );
  return temp == "Keep-Alive\r" || temp == "keep-alive\r" ? EConnection::eKeepAlive : EConnection::eClose;
}
