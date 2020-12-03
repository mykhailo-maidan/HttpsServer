#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#include "Socket.h"
#include <string>
#include <memory>


class ServerSocket 
{
public:

    explicit ServerSocket();
    ~ServerSocket();

    void start(uint64_t portNumber);

    Socket nextConnection();

private:
    int32_t createSocket(uint32_t port);

private:
    uint64_t serverFd{0};

    
};

#endif //_HTTPS_SERVER_H_
