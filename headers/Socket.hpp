#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <map>      
#include <string>  
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <poll.h>

#define MAX_CLIENTS 10

class Socket
{
private:
    int _serverSocket;
    sockaddr_in _adress_server;
public:
    Socket();
    Socket(std::map<std::string, std::string> config);
    Socket &operator=(const Socket &other);
    ~Socket();
    void bindingListening();
    void showSocket();
    void initPollFd();
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
