#include "ServerSocket.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <memory>


ServerSocket::ServerSocket()
{

}

ServerSocket::~ServerSocket()
{

}

void ServerSocket::start(uint64_t portNumber)
{
    serverFd = createSocket(portNumber);
}

Socket ServerSocket::nextConnection()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    auto client = accept(serverFd, reinterpret_cast<struct sockaddr*>(&addr),&len);

    return Socket(client);
}


int32_t ServerSocket::createSocket(uint32_t port)
{
    int32_t serverFd{-1};
    struct sockaddr_in addr;

    serverFd = socket(AF_INET,SOCK_STREAM,0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(serverFd, reinterpret_cast<sockaddr*>(&addr),sizeof(addr)) != 0)
    {
        perror("can't bind port");
        return -1;
    }

    if( listen(serverFd,10 ) != 0 )
    {
        perror("Can't configure listening port");
        return -1;
    }
    return serverFd;
}


