/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <openssl/rsa.h>
#include "TLSServerSocket.h"
#include "HttpParser.h"



int main()
{

    std::string request= "GET /uri.cgi HTTP/1.1\r\n"
                        "User-Agent: Mozilla/5.0\r\n"
                        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                        "Host: 127.0.0.1\r\n"
                        "\r\n";
        
    HttpParser parser(request);
    parser.parse();
/*  TLSServerSocket server;
    server.start(8080);

    auto client  = server.nextConnection();

    std::cout << client.read() << std::endl;

    client.write("Hello from new tls socket");
    */
    return 0;
}