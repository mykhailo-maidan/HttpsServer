#ifndef _TLS_SOCKET_H_
#define _TLS_SOCKET_H_

#include "openssl/ssl.h"
#include "openssl/err.h"

#include <memory>

class TLSSocket
{
public:
    TLSSocket( int32_t fd, std::shared_ptr<SSL_CTX> context );

    std::string read();

    void write(const std::string& message);
private:
    void showCerts();

private:
class SSLDeleter
{
    public:
    void operator()(SSL* p)
    {
        SSL_shutdown(p);
        SSL_free(p);
    }
};
    std::shared_ptr<SSL_CTX>          mContext{nullptr};
    std::unique_ptr<SSL,SSLDeleter>   mSocketDescriptor{nullptr};
};

#endif // _TLS_SOCKET_H_