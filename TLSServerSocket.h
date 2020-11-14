#ifndef _TLS_SERVER_SOCKET_H_
#define _TLS_SERVER_SOCKET_H_

#include "TLSSocket.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <string>
#include <memory>


class TLSServerSocket 
{
public:

    explicit TLSServerSocket();
    ~TLSServerSocket();

    void start(uint64_t portNumber);

    TLSSocket nextConnection();

private:
    int32_t createSocket(uint32_t port);

    std::shared_ptr<SSL_CTX> initServerCtx();

    void loadCertificates(SSL_CTX* ctx, char* certFile, char* keyFile);

    void clientHandler(SSL* ssl);

private:
    std::shared_ptr<SSL_CTX> context{nullptr};
    uint64_t serverFd{0};

    
};

#endif //_HTTPS_SERVER_H_
