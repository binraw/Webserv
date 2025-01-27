


/*============================================================================*/
                        /*### HEADER FILES ###*/
/*============================================================================*/
#include "Server.hpp"

/*============================================================================*/
                    /*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/
Server::Server(const std::vector<std::string> &)
//throw(std::exception)
  : _backLog(1024)
{
	setSocket();
// # ifdef TEST
// 	std::cout	<< BOLD CYAN << "\nServer	:\n"
// 				<< *this
// 				<< RESET << std::endl;
// # endif
}
/*----------------------------------------------------------------------------*/

Server::Server(const Server & ref)
  : _backLog(ref._backLog)
{	}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### DESTRUCTORS ###*/
/*============================================================================*/
Server::~Server() {
	closeFdSet();
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                    /*### OVERLOAD OPERATOR ###*/
/*============================================================================*/
Server  & Server::operator=(const Server &)
{ return *this; }
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Server & ref)
{
	o	<< BOLD CYAN << "SERVER :" << std::endl
		<< "_params :" << std::endl
		<< ref.getParams()
		<< "HERE\n";
	if (ref.getFdSet().empty() != true) {
		o << "NOT HERE" << std::endl << BOLD CYAN "_fdSet	: " << RESET CYAN;
		for (std::set<int>::iterator it = ref.getFdSet().begin(); \
			it != ref.getFdSet().end(); it ++)
			o << *it << " : ";
	}
	return o << RESET, o;
}
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const t_paramServer & ref)
{
	o << BOLD BRIGHT_CYAN "Map params :" << RESET BRIGHT_CYAN << std::endl;
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
			o << std::endl;
		}
		else
			o	<< "No value" << std::endl;
	}
	return o << RESET, o;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### GETTER ###*/
/*============================================================================*/

/*	* conversion en constant sur les membres non constant pour eviter les betises
	*
*/

const t_paramServer	& Server::getParams() const {
	return const_cast<t_paramServer&>(_params);
}
/*----------------------------------------------------------------------------*/

const std::set<int>	& Server::getFdSet() const {
	return const_cast<std::set<int>&>(_fdSet);
}
/*----------------------------------------------------------------------------*/

const int	& Server::getBacklog() const{
	return _backLog;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### SETTERS ###*/
/*============================================================================*/

/*	* initialise la variable _params
	*
*/
void	Server::setParams(std::vector<std::string> &)
{
	/* code */
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PRIVATE METHODS ###*/
/*============================================================================*/

void	Server::setSocket()// throw(InitException)
{
	struct addrinfo	hints, *res = NULL;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
# ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setSocket()"
				<< RESET << std::endl;
# endif
	for (std::set<std::string>::const_iterator it = _params.params["listen"].begin(); \
											it != _params.params["listen"].end(); it++)
	{
		int ret = getaddrinfo(NULL, it->c_str(), &hints, &res);
		if (ret != 0) {
			std::cerr	<< RED << gai_strerror(ret) << " on port [" << it->c_str() << "]"
						<< RESET << std::endl;
			closeFdSet();
			throw InitException(__FILE__, __LINE__, "Error -> getaddrinfo()", NULL);
		}
		for (struct addrinfo *currNode = res; currNode != NULL; currNode = currNode->ai_next)
		{
			int fd = -1;
			try {
				setSockOptSafe(currNode, fd);
				linkSocket(fd, currNode, it->c_str());
			}
			catch(const InitException &e) {
				fd > 0 ? close(fd) : fd;
				closeFdSet();
				freeaddrinfo(res);
				e.setSockExcept();
				throw;
			}
			_fdSet.insert(fd);
// # ifdef TEST
// 			std::cout	<< BLUE << "Socket : fd = " << fd << " at port " << it->c_str()
// 						<< RESET << std::endl;
// # endif
		}
		freeaddrinfo(res);
	}
}
/*----------------------------------------------------------------------------*/

#include <fcntl.h>
void	Server::setSockOptSafe(const struct addrinfo *currNode, int &fd)
const //throw(InitException)
{
	fd = socket(currNode->ai_family, currNode->ai_socktype | SOCK_NONBLOCK, currNode->ai_protocol);
	if (fd < 0) {		
		throw InitException(__FILE__, __LINE__ - 2, "Error -> socket()", NULL);
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) != 0) // R T MANUAL
		std::cerr << "FCNLT\n";

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL);
	}
}
/*----------------------------------------------------------------------------*/

void	Server::linkSocket(const int sockFd, const struct addrinfo * currNode, const char *currPort)
const //throw(InitException)
{
	if (bind(sockFd, currNode->ai_addr, currNode->ai_addrlen) != 0) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> bind()", currPort);
	}
	// socket en mode écoute pour les connexions entrantes,
	// spécifiant le nombre maximum de connexions en attente BACKLOG.
	if (listen(sockFd, DFLT_BACKLOG ) != 0) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> listen()", currPort);
	}
}
/*----------------------------------------------------------------------------*/

void	Server::closeFdSet()
{
	for (std::set<int>::iterator it = _fdSet.begin(); it != _fdSet.end(); it++)
		if (*it > 0 && close(*it) != 0)
			std::cerr	<< RED "Error when closing fd " << *it
						<< RESET << std::endl;
	_fdSet.clear();
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PUBLIC METHODS ###*/
/*============================================================================*/

/*	* PARSING METHODS
	*
*/

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
/*----------------------------------------------------------------------------*/

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
/*----------------------------------------------------------------------------*/

/*	* mettre une erreur si different de 9 en size en comptant 'server'
	*
*/
void Server::createMapDefaultConf()
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
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                            /*### EXCEPTIONS ###*/
/*============================================================================*/

void	Server::InitException::setSockExcept() const throw() {
	std::cerr	<< RED << strerror(errno)
				<< std::endl
				<< YELLOW "at file [" << _file << "] line [" << _line << "]";
	if (_port != NULL) {
		std::cerr	<< " ( port [" << _port << "] )";
	}
	std::cerr	<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

const char *	Server::InitException::what() const throw() {
	return _msg;
}
/*----------------------------------------------------------------------------*/
