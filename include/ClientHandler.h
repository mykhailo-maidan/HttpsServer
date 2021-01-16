#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include <chrono>
#include "IOSocket.h"
#include "HttpParser.h"

class ClientHandler
{
public:
  ClientHandler( IOSocket socket, const std::string& rootFolder );

  ~ClientHandler() = default;

  void handle();

private:
  std::string readRequest();
  void        requestHandler( const HttpParser::SHeader& request );
  void        handleGetRequest( const HttpParser::SHeader& request, bool keepAlive );
  void        uploadResourse( const std::string& url, bool keepAlive);
  void        closeConnectionHandle(const HttpParser::SHeader& request);
  void        keepAliveConnectionHandle(const HttpParser::SHeader& request);
  void        handleRequestMethod(const HttpParser::SHeader&  request);

private:
  IOSocket    mSocket;
  std::string mRootFolder;
};

#endif //  _CLIENT_HANDLER_H_