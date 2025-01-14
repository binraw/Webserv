#include "../headers/Socket.hpp"

Socket::Socket()
{
    int port = 8080;
    _serverSocket = 0;
    // _serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = utilisation IPV4 , SOCK_STREAM = TCP
      // et 0 par default pour TCP
    _adress_server.sin_family = AF_INET;

    _adress_server.sin_port = htons(port);
    _adress_server.sin_addr.s_addr = INADDR_ANY;
}

Socket::Socket(std::map<std::string, std::string> config)
{
    int port;
    _serverSocket = 0;
    _adress_server.sin_family = AF_INET;
    std::istringstream iss(config.at("port"));
    if (!iss)
        throw PortNotDefine();
    iss >> port;
    std::cout << "Value de port :" << port << std::endl;
    if (port < 1024 || port > 65535)
        throw PortNotDisponible();
    _adress_server.sin_port = htons(port);
    _adress_server.sin_addr.s_addr = INADDR_ANY;
}

Socket &Socket::operator=(const Socket &other)
{
    if (this != &other)
    {
        this->_adress_server.sin_addr = other._adress_server.sin_addr;
        this->_adress_server.sin_family = other._adress_server.sin_family;
        this->_adress_server.sin_port = other._adress_server.sin_port;
        this->_serverSocket = other._serverSocket;
    }
    return *this;
}


Socket::~Socket()
{
}

void Socket::showSocket()
{
    std::cout << "adress sin family : " << this->_adress_server.sin_family << std::endl;
    std::cout << "adress port : " << this->_adress_server.sin_port << std::endl;
}

void Socket::bindingListening()
{
    int addrlen = sizeof(_adress_server);
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(_serverSocket, (struct sockaddr*)&_adress_server, sizeof(_adress_server));
    listen(_serverSocket, 5); // voir doc pour si 5 suffisant ou pas
    // int new_socket = accept(_serverSocket, (struct sockaddr *)&address, (stocklen_t *)&addrlen);
}

void Socket::initPollFd()
{
    fds[0].fd = _serverSocket;
    fds[0].events = POLLIN; // is data to read
    for (int i = 1; i <= MAX_CLIENTS; i++)
    {
        fds[i].fd = -1;
    }
}


const char* Socket::FailedSocket::what() const throw()
{
    return "Failed to create Socket";
}

const char* Socket::PortNotDefine::what() const throw()
{
    return "Port not define on server.conf";
}

const char* Socket::PortNotDisponible::what() const throw()
{
    return "Port must be between 1024 and 65535.";
}

