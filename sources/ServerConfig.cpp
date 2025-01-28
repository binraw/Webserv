#include "ServerConfig.hpp"

void ServerConfig::displayValueServer()
{
    // std::cout <<  _clientMaxBodySize << std::endl;
    displayVector(_serverName);
    displayVector(_listenPort);
}

void ServerConfig::displayVector(std::vector<std::string> vec)
{
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
        std::cout << "Value vector : " << *it << std::endl;
}
void ServerConfig::cleanVector() // clean les vector de leur key je sais ps encore comment faire
{

}