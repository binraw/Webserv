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
    // _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // if (_serverSocket < 0) 
    //     throw FailedSocket();
    _adress_server.sin_family = AF_INET;
    std::istringstream iss(config.at("port"));
    if (!iss)
        throw PortNotDefine();
    iss >> port;
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
        this->_adress_server.sin_addr.s_addr = other._adress_server.sin_addr.s_addr;
        this->_adress_server.sin_port = other._adress_server.sin_addr;
        this->_serverSocket = other._serverSocket;
    }
    return *this;
}


Socket::~Socket()
{
}

void Socket::bindingListening()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(_serverSocket, (struct sockaddr*)&_adress_server, sizeof(_adress_server));
    listen(_serverSocket, 5); // voir doc pour si 5 suffisant ou pas
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

try
{
    Socket sock
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
