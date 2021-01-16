#ifndef _HTTP_RESPONSE_BUILDER_H_
#define _HTTP_RESPONSE_BUILDER_H_

#include <string>

class HttpResponseBuilder;

using Self = HttpResponseBuilder;

class HttpResponseBuilder
{
public:
  enum class EResponseCode : uint16_t
  {
    eOK             = 200,
    eBadRequest     = 400,
    eNotFound       = 404,
    eNotImplemented = 501
  };

  enum class EConnection : uint8_t
  {
    eClose,
    eKeepAlive
  };

  enum class ETransferEncoding: uint8_t
  {
    eChunked
  };

  HttpResponseBuilder()  = default;
  ~HttpResponseBuilder() = default;

  Self& version( const std::string& version );
  Self& code( EResponseCode code );
  Self& connection( EConnection connection );
  Self& server( const std::string& server );
  Self& keepAlive( uint32_t timeout, uint32_t max = 500 );
  Self& transferEncoding(ETransferEncoding encoding);
        operator std::string();

private:

  std::string getData();

private:
  std::string mResponse;
  std::string mConnectionType;
  std::string mKeepAlive;
  std::string mServerName;
  std::string mTransferEncoding;
};

#endif // _HTTP_RESPONSE_BUILDER_H_