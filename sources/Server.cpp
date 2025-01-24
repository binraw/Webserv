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
    int number_road = 0;
    for(it = _basicData.begin(); it != _basicData.end(); it++)
    {
        if (*it == std::string("{") || *it == std::string("location"))
            i++;
        if (*it == std::string("location")) // ici changer pour les localisation j'imagine
        {
            std::pair<int, std::vector<std::string> > serverPair(number_road, addValuesRoads(it));
            _vectRoads.insert(serverPair);
            number_road++;
        }
        if (*it == std::string("}"))
            i = i - 2;
        if (i <= 1)
            _defaultConfServer.push_back(*it);
    }
    _defaultConfServer = UtilParsing::cleanVector(_defaultConfServer);
    createMapDefaultConf();
    createMapRoads();
    UtilParsing::printMapVector(_mapRoads);
    // std::cout << "VALUES  DEFAULT SERVER :" <<  std::endl;
    //   for (size_t y = 0; y < _defaultConfServer.size(); y++) {
    //     std::cout << _defaultConfServer[y] << std::endl;
    // }

}
std::vector<std::string> Server::addValuesRoads(std::vector<std::string>::iterator &cursor)
{
    std::vector<std::string> road;

    for(;cursor != _basicData.end(); cursor++)
    {
        if (*cursor == std::string("}"))
            break;
        road.push_back(*cursor); 
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


// reflexion sur comment faire pour faire la meme fonction que pour cluster mais pour la localisation 
// des roads car les pattern ne sont pas les memes car les deux premier du vector sont forcement ensemble 
// mais jai limpression que dans le vector ca peut etre pas forcement dans le bon ordre donc piste a verifier


void Server::createMapRoads()
{
    if (_vectRoads.empty()) 
        return;

    std::string key;
    std::string keystr;
    std::string valuestr;
    std::vector<std::string> params;

    // for (std::vector<std::string>::iterator tic = _vectRoads[0].begin(); tic != _vectRoads[0].end(); tic++)
    // {
    //     std::cout << *tic << std::endl;
    //     // key = *tic;
    //     // params.push_back(*(tic + 1));
    //     // std::pair<std::string, std::vector<std::string> > serverPair(key, params);
    //     // _mapRoads.insert(serverPair);
    //     // params.clear();
    //     // break ; 
    // }
    // for (std::vector<std::string>::iterator tic = _vectRoads[0].begin(); tic != _vectRoads[0].begin(); tic++)
    // {
    //     std::cout << "RENTRE" << std::endl;
    //     key = *tic;
    //     params.push_back(*(tic + 1));
    //     std::pair<std::string, std::vector<std::string> > serverPair(key, params);
    //     _mapRoads.insert(serverPair);
    //     params.clear();
    //     // break ; 
    // }

  
    for (std::vector<std::string>::iterator it = _vectRoads[0].begin(); it != _vectRoads[0].end(); ++it)
    {
        valuestr = *it;
        if (it != _vectRoads[0].begin())
        {
            std::vector<std::string>::iterator prev = it - 1;
            keystr = *prev;
            if (valuestr.find(';') == std::string::npos && keystr.find(';') != std::string::npos)
                key = *it;
        }
        else if (it == _vectRoads[0].begin() || valuestr.find(';') == std::string::npos)
        {
            key = *it;
        }
        for (std::vector<std::string>::iterator tic = it + 1; tic != _vectRoads[0].end(); tic++)
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
            _mapRoads.insert(serverPair);
            params.clear();
    }
}
