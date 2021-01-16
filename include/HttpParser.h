#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include <string>
#include <utility>
#include <optional>

class HttpParser
{
public:
  enum class EMethod
  {
    eGet,
    eNone
  };

  enum class EConnection
  {
    eClose,
    eKeepAlive
  };

  struct SHeader
  {
    EMethod                       method;
    std::string                   url;
    std::string                   version;
    std::string                   user_agent;
    std::string                   accept;
    std::string                   host;
    EConnection                   connection;
  };

public:
  explicit HttpParser( const std::string& data );
  ~HttpParser();

  std::optional<SHeader> parse();

private:
  std::string mDataForParsing;

private:
  void        parseMethod( const std::string& token, SHeader& header );
  EConnection parseConnection( const std::string& data );
};

#endif // _HTTP_PARSER_H_