



#include "Server.hpp"

#include <algorithm>

void	print(std::string & msg) {
	std::cout << msg << std::endl;
}

Server::Server(const std::vector<std::string> & data)
  : _protoName(getprotobyname("tcp"))
{
	// std::for_each(data.begin(), data.end(), print);
	// std::cout << *this;
	/*
		trouver le champ "listen" dans la map params
		si il n'est pas trouve il cree le champ listen et lui assigne une valeur par defaut
	*/

}

Server::Server(const Server & ref)
{   }

Server::~Server()
{   }

Server  & Server::operator=(const Server & ref)
{ return *this; }

std::ostream	& operator<<(std::ostream & o, const Server & ref)
{
	o	<< "_params :" << std::endl
		<< ref.getParams();
	return o;
}

std::ostream	& operator<<(std::ostream & o, const t_paramServer & ref)
{
	for (std::map<std::string, std::set<std::string> >::const_iterator it = ref.params.begin(); \
		it != ref.params.end(); it++)
	{
		std::set<std::string>::const_iterator itt = it->second.begin();
		o	<< "key	: [" << it->first << "]" << std::endl;
		if (itt != it->second.end()) {
			o	<< "values	: [" << *itt << "]" << std::endl;
			for (++itt; itt != it->second.end(); itt++) {
				o	<< "values	: [" << *itt << "]" << std::endl;
			}
		}
		else
			o	<< "No value";
		o << std::endl;
	}
	return o;
}

// GETTERS //
t_paramServer	& Server::getParams() const
{ return const_cast<t_paramServer&>(_params); } // conversion d'un non constant en constant pour eviter les betises


// SETTERS //
void            Server::setParams()
{
	// if (!UtilParsing::isKeyExist<std::string, std::set<std::string> >(_params.params, "listen") \
	// 	|| !UtilParsing::isvalueExist<std::string, std::string>(_params.params, "listen", DFLT_LISTENPORT))
	// {
	// 	std::set<std::string> value;
	// 	value.insert(DFLT_LISTENPORT);
	// 	_params.params["listen"] = value;
	// 	std::cout	<< YELLOW << "info : port " DFLT_LISTENPORT << " defined as default listenning port" 
	// 				<< RESET << std::endl;
	// }
	
	// for (std::set<std::string>::iterator it = _params.params["listen"].begin(); it != _params.params["listen"].end(); it++)
	// {
	// 	std::cout << *it << std::endl;

	// }

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
	// std::cout << "VALUES  DEFAULT SERVER :" <<  std::endl;
	//   for (size_t y = 0; y < _defaultConfServer.size(); y++) {
	//	 std::cout << _defaultConfServer[y] << std::endl;
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
	//	 std::cout << road[x] << std::endl;
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
		//	 std::cout << tic->first << " " << tic->second << std::endl;
		// }
}
