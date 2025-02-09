
#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd)
  : _clientSocket(fd)
{}

Client::Client(const Client &ref)
  : _clientSocket(ref._clientSocket)
{
    // if (ref._request)
    //     _request = ref._request.duplicate();
}

Client::~Client()
{
    if (_clientSocket > 0)
        close(_clientSocket);
    if (_request)
        delete _request;
}

