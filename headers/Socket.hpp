#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

class Socket
{
private:
    int _serverSocket;
    sockaddr_in _adress_server;
public:
    Socket();
    Socket(std::map<std::string, std::string> config);
    Socket Socket &operator=(const Socket &other);
    ~Socket();
    void bindingListening();
    class FailedSocket: public std::exception
    {
        virtual const char* what() const throw();
    };
    class PortNotDefine: public std::exception
    {
        virtual const char* what() const throw();
    };
    class PortNotDisponible: public std::exception
    {
        virtual const char* what() const throw();
    };
};






#endif