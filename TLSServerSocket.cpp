#include "TLSServerSocket.h"
#include "TLSSocket.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <memory>

namespace 
{
    class SSLCTXDeleter
    {
    public:
        void operator()(SSL_CTX* p)
        {
            SSL_CTX_free(p);
        }
    }
}

TLSServerSocket::TLSServerSocket()
{

}

TLSServerSocket::~TLSServerSocket()
{

}

void TLSServerSocket::start(uint64_t portNumber)
{

    SSL_library_init();
    context = initServerCtx();

    loadCertificates(context.get(), "mycert.pem","mycert.pem");

    serverFd = createSocket(portNumber);
}

TLSSocket TLSServerSocket::nextConnection()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    auto client = accept(serverFd, reinterpret_cast<struct sockaddr*>(&addr),&len);

    return TLSSocket(client, context);
}


int32_t TLSServerSocket::createSocket(uint32_t port)
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


std::shared_ptr<SSL_CTX> TLSServerSocket::initServerCtx()
{
    const SSL_METHOD* method{nullptr};
    std::shared_ptr<SSL_CTX> ctx{nullptr};
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLSv1_2_server_method();
    ctx = std::shared_ptr<SSL_CTX>(SSL_CTX_new(method),SSLCTXDeleter());

    if( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    SSL_CTX_set_cipher_list(ctx.get(),"ALL:eNULL");

    return ctx;

}

void TLSServerSocket::loadCertificates(SSL_CTX* ctx, char* certFile, char* keyFile)
{
    if(SSL_CTX_use_certificate_file(ctx,certFile,SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if(SSL_CTX_use_PrivateKey_file(ctx,keyFile,SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    if(!SSL_CTX_check_private_key(ctx))
    {
        fprintf(stderr,"Private key does not  match the public certificate\n");
        abort();
    }
}



void TLSServerSocket::clientHandler(SSL* ssl)
{
    char buff[1024] = {0};
    int bytes { 0 };

    constexpr auto response = "response from server";

    if( SSL_accept(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);

    }
    {
        bytes = SSL_read(ssl, buff, sizeof(buff));
        buff[bytes] = '\0';

        printf("Client message:%s\n",buff);

        if( bytes > 0)
        {
            SSL_write(ssl, response, strlen(response) );
        }
        else 
        {
            ERR_print_errors_fp(stderr);
        }
    }

    int sd = SSL_get_fd(ssl);
    SSL_free(ssl);
    //close(sd);
}

