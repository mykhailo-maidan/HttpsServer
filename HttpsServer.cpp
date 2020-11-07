#include "HttpsServer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>



HttpsServer::HttpsServer(uint32_t port)
{

}

HttpsServer::~HttpsServer()
{

}

int32_t HttpsServer::createSocket(uint32_t port)
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


SSL_CTX* HttpServer::initServerCtx()
{
    const SSL_METHOD* method{nullptr};
    SSL_CTX* ctx{nullptr};
    OpenSSL_add_all_algorithms();
    SLL_load_error_strings();

    method = TLSv1_2_server_method();
    ctx = SSL_CTX_new(method);

    if( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    SLL_CTX_set_cipher_list(ctx,"ALL:eNULL");

    return ctx;

}

void HttpsServer::loadCertificates(SLL_CTX* ctx, char* certFile, char* keyFile)
{

}
