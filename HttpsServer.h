#ifndef _HTTPS_SERVER_H_
#define _HTTPS_SERVER_H_

#include "openssl/ssl.h"
#include "openssl/err.h"

#include <string>

class HttpsServer 
{
public:
    explicit HttpsServer();
    ~HttpsServer();

    void start(uint64_t portNumber);

private:
    int32_t createSocket(uint32_t port);

    SSL_CTX* initServerCtx();

    void loadCertificates(SSL_CTX* ctx, char* certFile, char* keyFile);

    void showCerts(SSL* ssl);

    void clientHandler(SSL* ssl);

private:
    SSL_CTX* context{nullptr};
    uint64_t serverFd{0};

    
};

#endif //_HTTPS_SERVER_H_
