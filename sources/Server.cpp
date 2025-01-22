#include "Server.hpp"

Server::Server(/* const std::vector<std::string> & data */)
  : _protoName(getprotobyname("tcp"))
{
    // ouvrir tous les sockets necessaire (autant qu'il y a de port d'ecoute)
    
	/*
		trouver le champ "listen" dans la map params
		si il n'est pas trouve il cree le champ listen et lui assigne une valeur par defaut
	*/	
	std::map<std::string, std::vector<std::string> >::iterator
		it = _params.params.find("listen");
	if (it == _params.params.end())
	{
		std::string					key = "listen";
		std::vector<std::string>	value = { DFLT_LISTENPORT };
		_params.params.insert(std::make_pair(key, value));
		std::cout	<< YELLOW << "info :" << std::endl
					<< "listen field added in _params server with 8000 as default value"
					<< RESET << std::endl;
		it = _params.params.find("listen"); // verifier le retour de fonction
	}
	
	/*
		verifie si il y a bien un port par defaut sinon il l'ajoute
	*/
	std::vector<std::string>::iterator
		itt = it->second.begin();
	if (itt == it->second.end()) {
		try {
			it->second.push_back(DFLT_LISTENPORT);
		}
		catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			return ;
		}
		std::cout	<< YELLOW << "info :" << std::endl
					<< "no values ​​assigned to listen field, set by default to port "
					<< DFLT_LISTENPORT
					<< RESET << std::endl;
		itt = it->second.begin(); // verifier le retour de fonction
	}

	// loop to create all socket
	for (itt; itt != it->second.end(); itt++)
	{
		std::cout << *itt << std::endl;
		
	}

}

Server::Server(const Server & ref)
{   }

Server::~Server()
{   }

Server  & Server::operator=(const Server & ref)
{   }

std::ostream	& operator<<(std::ostream &, const Server &)
{   }

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
    // std::cout << "VALUES  DEFAULT SERVER :" <<  std::endl;
    //   for (size_t y = 0; y < _defaultConfServer.size(); y++) {
    //     std::cout << _defaultConfServer[y] << std::endl;
    // }

}

std::vector<std::string> Server::addValuesRoads(std::vector<std::string>::iterator &cursor)
{
    std::vector<std::string> road;
    std::vector<std::string>::iterator it;
    int i = 0;
    for(it = cursor;it != _basicData.end(); it++)
    {
        if (*it == std::string("}"))
            i++;
        if (i > 0)
            break;
        road.push_back(*it);
        
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
