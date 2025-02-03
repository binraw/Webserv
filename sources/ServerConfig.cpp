#include "ServerConfig.hpp"

void ServerConfig::displayValueServer()
{
    std::cout <<  _clientMaxBodySize << std::endl;
    std::cout <<  _uploadPath << std::endl;
    UtilParsing::displayVector(_serverName);
    UtilParsing::displayVector(_listenPort);
    displayAllLocations();
}




void ServerConfig::displayAllLocations()
{
    for(std::vector<LocationConfig>::iterator it = _locations.begin(); it != _locations.end(); it++)
    {
        it->displayLocation();
    }
}
