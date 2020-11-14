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
    std::shared_ptr<SSL_CTX> mContext{nullptr};
    std::shared_ptr<SSL>     mSocketDescriptor{nullptr};


};

#endif // _TLS_SOCKET_H_