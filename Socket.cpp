#include "Socket.h"
#include <iostream>
#include <unistd.h>


Socket::Socket(int32_t fd): fd(fd)
{

}

std::string Socket::receive()
{
    char buff[1024] = { 0 };
    auto bytes = read(fd, buff, sizeof(buff));
    buff[bytes] = '\0';
    std::cout << buff << std::endl;
    return std::string(buff);
}

void Socket::send(const std::string& message)
{
    write(fd,message.c_str(), message.size());
}
