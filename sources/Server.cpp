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
    _numberRoads = 0;
    for(it = _basicData.begin(); it != _basicData.end(); it++)
    {
        if (*it == std::string("{") || *it == std::string("location"))
            i++;
        if (*it == std::string("location"))
        {
            std::pair<int, std::vector<std::string> > serverPair(_numberRoads, addValuesRoads(it));
            _vectRoads.insert(serverPair);
            _numberRoads++;
        }
        if (*it == std::string("}"))
            i = i - 2;
        if (i <= 1)
            _defaultConfServer.push_back(*it);
    }
    _defaultConfServer = UtilParsing::cleanVector(_defaultConfServer);
    createMapDefaultConf();
    for (int y = 0; y < _numberRoads; y++)
        createMapRoads(y);
    
    UtilParsing::manageControlMapLocation(_allMapRoads);
    // UtilParsing::printMapVector(_allMapRoads);
    // std::cout << "VALUES  DEFAULT SERVER :" <<  std::endl;
    //   for (size_t y = 0; y < _defaultConfServer.size(); y++) {
    //     std::cout << _defaultConfServer[y] << std::endl;
    // }

}
std::vector<std::string> Server::addValuesRoads(std::vector<std::string>::iterator &cursor)
{
    std::vector<std::string> road;

    for( ;cursor != _basicData.end(); ++cursor)
    {
        if (*cursor == std::string("}"))
            break;
        road.push_back(*cursor);
        // std::cout << *cursor << std::endl;
    }
    road = UtilParsing::cleanVector(road);
    // std::cout << "Value de la localisation :" << std::endl;
    // for (size_t x = 0; x < road.size(); x++) 
    // {
    //     std::cout << road[x] << std::endl;
    // }
    return road;
}



void Server::createMapDefaultConf() // mettre une erreur si different de 9 en size en comptant 'server'
{
        for (std::vector<std::string>::iterator it = _defaultConfServer.begin() + 1; it != _defaultConfServer.end(); )
        { // je begin + 1 pour skip 'server'
            std::string key = *it;
            it++;
            if (it != _defaultConfServer.end())
            {
            std::pair<std::string,std::string> serverPair(key, *it);
              _mapConfDefault.insert(serverPair);
              ++it;
            }
        }
        // for (std::map<std::string, std::string>::iterator tic = _mapConfDefault.begin(); tic != _mapConfDefault.end(); ++tic)
        // {
        //     std::cout << tic->first << " " << tic->second << std::endl;
        // }
}



void Server::createMapRoads(int nb)
{
    if (_vectRoads[nb].empty()) 
        return;

    std::string key;
    std::string keystr;
    std::string valuestr;
    std::vector<std::string> params;
  
    for (std::vector<std::string>::iterator it = _vectRoads[nb].begin(); it != _vectRoads[nb].end(); it++)
    {
        valuestr = *it;
        if (it != _vectRoads[nb].begin())
        {
            std::vector<std::string>::iterator prev = it - 1;
            keystr = *prev;
            if (valuestr.find(';') == std::string::npos && keystr.find(';') != std::string::npos)
                key = *it;
        }
        else if (it == _vectRoads[nb].begin() /*|| valuestr.find(';') == std::string::npos*/)
        {
            key = *it;
        }
        for (std::vector<std::string>::iterator tic = it + 1; tic != _vectRoads[nb].end(); tic++)
        {
            valuestr = *(tic - 1);
            keystr = *tic;
            if (tic->find(";") == std::string::npos && valuestr.find("location") != std::string::npos)
            {
                params.push_back(*tic);
                break;
            }
            if (tic->find(";") != std::string::npos)
            {
                params.push_back(*tic);
            }
            else if (valuestr.find(";") == std::string::npos)
            {
                params.push_back(*tic);
            }
            else
                break;
        }
            std::pair<std::string, std::vector<std::string> > serverPair(key, params);
            _allMapRoads[nb].insert(serverPair);
            params.clear();
    }
}









void s_DefaultParamsServer::addValuesParamsServerDefault(std::map<std::string, std::vector<std::string> > mapRecover)
{
    for (std::map<std::string, std::vector<std::string> >::iterator it = mapRecover.begin(); it != mapRecover.end(); it++)
    {
        const std::string &key = it->first;
        const std::vector<std::string> &value = it->second;
        if (params.find(key) != params.end())
            params[key].insert(params[key].end(), value.begin(), value.end()); // rajout des values a la suite
        else
            params[key] = value; // la en gros si la key n'existe pas de base dans la map default on la rajoute
    }
}

void s_DefaultParamsServer::printStructMapDefault()
{
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = params.begin(); 
         it != params.end(); 
         ++it)
    {
        const std::string& key = it->first;
        const std::vector<std::string>& value = it->second;

        std::cout << "Key: " << key << "\nParams: ";

        for (std::vector<std::string>::const_iterator paramIt = value.begin(); 
             paramIt != value.end(); 
             ++paramIt)
        {
            std::cout << *paramIt << " ";
        }

        std::cout << std::endl;
    }
}
