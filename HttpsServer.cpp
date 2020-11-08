#include "HttpsServer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>



HttpsServer::HttpsServer()
{

}

HttpsServer::~HttpsServer()
{

}

void HttpsServer::start(uint64_t portNumber)
{
    SSL_library_init();

    context = initServerCtx();

    loadCertificates(context, "mycert.pem","mycert.pem");

    serverFd = createSocket(portNumber);

    while(true)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);

        SSL* ssl;

        int client = accept(serverFd, reinterpret_cast<struct sockaddr*>(&addr),&len);
        printf("Connection:%s:%d\n", inet_ntoa(addr.sin_addr), htons(addr.sin_port));
        ssl = SSL_new(context);
        SSL_set_fd(ssl,client);
        clientHandler(ssl);
    }
    //close(serverFd);
    SSL_CTX_free(context);
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


SSL_CTX* HttpsServer::initServerCtx()
{
    const SSL_METHOD* method{nullptr};
    SSL_CTX* ctx{nullptr};
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLSv1_2_server_method();
    ctx = SSL_CTX_new(method);

    if( ctx == nullptr )
    {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    SSL_CTX_set_cipher_list(ctx,"ALL:eNULL");

    return ctx;

}

void HttpsServer::loadCertificates(SSL_CTX* ctx, char* certFile, char* keyFile)
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

void HttpsServer::showCerts(SSL* ssl)
{
    X509* cert;
    char* line;

    cert = SSL_get_peer_certificate(ssl);
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

void HttpsServer::clientHandler(SSL* ssl)
{
    char buff[1024] = {0};
    int bytes { 0 };

    constexpr auto response = "response from server";

    if( SSL_accept(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);

    }
    {
        showCerts(ssl);
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

