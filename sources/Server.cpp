#include "Server.hpp"

Server::Server(std::vector<std::string> data)
{
    _basicData = data;
    initDefaultConfServ();

}

Server::~Server()
{

}

void Server::initDefaultConfServ()
{
    std::vector<std::string>::iterator it;
    int i = 0;
    // int number_servers = 0;
    for(it = _basicData.begin(); it != _basicData.end(); it++)
    {
        if (*it == std::string("{") || *it == std::string("location"))
            i++;
        if (*it == std::string("///////")) // ici changer pour les localisation j'imagine
        {
            // std::cout << " RENTRE " << std::endl;
            // //faut utiliser pair pour creer des paire comme ca avec des maps tres style
            // std::pair<int, std::vector<std::string> > serverPair(number_servers, addValuesServers(it));
            // _vectServers.insert(serverPair);
            // _defaultConf.pop_back();
            // number_servers++;
        }
        if (*it == std::string("}"))
            i = i - 2;
        if (i <= 1)
            _defaultConfServer.push_back(*it);
    }
    _defaultConfServer = UtilParsing::cleanVector(_defaultConfServer);
    std::cout << "VALUES  DEFAULT SERVER :" <<  std::endl;
      for (size_t y = 0; y < _defaultConfServer.size(); y++) {
        std::cout << _defaultConfServer[y] << std::endl;
    }

}
