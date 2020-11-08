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
#include "HttpsServer.h"



int main()
{
    HttpsServer server;
    server.start(8080);
    return 0;



}