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
#include "ServerSocket.h"



int main()
{

    
    ServerSocket server;
    server.start(8080);

    auto client  = server.nextConnection();

    std::cout << client.receive() << std::endl;

    client.send("Hello from new tls socket");
    return 0;
}