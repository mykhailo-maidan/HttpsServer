#include "TLSSocket.h"
#include <iostream>

TLSSocket::TLSSocket(int32_t fd, std::shared_ptr<SSL_CTX> context):
    mContext(context),
    mSocketDescriptor(SSL_new(mContext.get()))
{
    SSL_set_fd(mSocketDescriptor.get(),fd);

    if(SSL_accept(mSocketDescriptor.get()) == -1)
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        showCerts();
    }

}

std::string TLSSocket::read()
{
    char buff[1024] = { 0 };
    auto bytes = SSL_read(mSocketDescriptor.get(), buff, sizeof(buff));
    buff[bytes] = '\0';
    std::cout << buff << std::endl;
    return std::string(buff);
}

void TLSSocket::write(const std::string& message)
{
    SSL_write(mSocketDescriptor.get(),message.c_str(), message.size());
}
void TLSSocket::showCerts()
{
    X509* cert;
    char* line;

    cert = SSL_get_peer_certificate(mSocketDescriptor.get());
    if(cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert),0,0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_subject_name(cert),0,0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
    {
        printf("No certificates.\n");
    }
}