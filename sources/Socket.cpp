#include "Socket.hpp"

Socket::Socket()
{
    int port = 8080;
    _clientCount = 0;
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
    _clientCount = 0;
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
        this->_clientCount = other._clientCount;
        for (int i = 0; i < MAX_CLIENTS + 1; ++i) 
            this->_fds[i] = other._fds[i];
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
    if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw FailedSocket();
    }

    // Permet la réutilisation de l'adresse
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw FailedSocket();
    }

    if (bind(_serverSocket, (struct sockaddr*)&_adress_server, sizeof(_adress_server)) < 0) {
        throw FailedSocket();
    }

    if (listen(_serverSocket, 5) < 0) {
        throw FailedSocket();
    }
}

void Socket::initPollFd()
{
    _fds[0].fd = _serverSocket;
    _fds[0].events = POLLIN; // is data to read
    for (int i = 1; i <= MAX_CLIENTS; i++)
    {
        _fds[i].fd = -1;
    }

    std::cout << "Server listening on port " <<  std::endl;
    while (true) 
    {
        int poll_count = poll(_fds, MAX_CLIENTS + 1, -1);
        if (poll_count < 0)
        {
            perror("poll error");
            break;
        }
        if (_fds[0].revents & POLLIN) // comparaison des bits si non null alors revents contient l'event chercher
            acceptConnection();
        for (int i = 1; i <= MAX_CLIENTS; ++i)
            if (_fds[i].fd != -1 && (_fds[i].revents & POLLIN)) 
                handleClient(i);
    }
}

void Socket::acceptConnection() 
{
    if (_clientCount >= MAX_CLIENTS) 
    {
        std::cout << "Max clients reached. Cannot accept new connections." << std::endl;
        return;
    }

    int new_socket = accept(_serverSocket, NULL, NULL);
    if (new_socket >= 0) 
    {
        std::cout << "New connection accepted" << std::endl;
        for (int i = 1; i <= MAX_CLIENTS; ++i)
        {
            if (_fds[i].fd == -1) 
            {
                _fds[i].fd = new_socket;
                _fds[i].events = POLLIN;
                _clientCount++;
                break;
            }
        }
    }
}

void Socket::handleClient(int clientIndex)
{
    char buffer[1024] = {0};
    int valread = read(_fds[clientIndex].fd, buffer, sizeof(buffer));
    if (valread > 0) 
    {
        std::cout << "Received request: " << buffer << std::endl;
        // const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello World!";
       const char *response = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, World!";
        send(_fds[clientIndex].fd, response, strlen(response), 0);
    }
    else 
    {
        std::cout << "Client disconnected" << std::endl;
        close(_fds[clientIndex].fd);
        _fds[clientIndex].fd = -1; // Marquer comme inactif
        _clientCount--;
    }
}

// void Socket::run()
// {
//     std::cout << "Server listening on port " << PORT << std::endl;
//     while (true) 
//     {
//         int poll_count = poll(fds, MAX_CLIENTS + 1, -1);
//         if (poll_count < 0)
//         {
//             perror("poll error");
//             break;
//         }
//         if (fds[0].revents & POLLIN) // comparaison des bits si non null alors revents contient l'event chercher
//             acceptConnection();
//         for (int i = 1; i <= MAX_CLIENTS; ++i)
//             if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) 
//                 handleClient(i);
//     }
// }


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

