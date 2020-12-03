#ifndef _SOCKET_H_
#define _SOCKET_H_


#include <memory>

class Socket
{
public:
    Socket( int32_t fd );

    std::string receive();

    void send(const std::string& message);
private:
  uint64_t fd;
};

#endif // _SOCKET_H_