#ifndef _HTTPS_SERVER_H_
#define _HTTPS_SERVER_H_

#include "openssl/ssl.h"
#include "openssl/err.h"

#include <string>

class HttpsServer 
{
public:
    explicit HttpsServer( uint32_t port );
    ~HttpsServer();

private:
    int32_t createSocket(uint32_t port);

    SSL_CTX* initServerCtx();

    loadCertificates(SLL_CTX* ctx, char* certFile, char* keyFile);
    
}

#endif //_HTTPS_SERVER_H_
