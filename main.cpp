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




int main()
{

    int server_fd;
    int new_socket;
    int valread;
    int opt  =1;

    struct sockaddr_in address;
    struct sockaddr_in clien_address;
    socklen_t clientLen;

    auto addlen = sizeof(address);
    char buffer[1024] = {0};

    if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == 0)
    {
        std::cout << "Socket Failed\n";
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd,(struct sockaddr*)(&address),sizeof(address))< 0 )
    {
        std::cout << "bind failed\n";
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,5) < 0 )
    {
        std::cout << "listen\n";
        exit(EXIT_FAILURE);
    }
    clientLen = sizeof(clien_address);

    new_socket = accept(server_fd,(struct sockaddr*)&clien_address,&clientLen);

    std::cout << "New Connection" << new_socket << std::endl;
//    send(new_socket, "Hello, world!\n", 13, 0);
    valread = read(new_socket,buffer,sizeof(buffer));
    printf("%s\n",buffer);
    send(new_socket, "Hello, world!\n", 13, 0);
    return 0;



}