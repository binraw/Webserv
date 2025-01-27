


/*============================================================================*/
                        /*### HEADER FILES ###*/
/*============================================================================*/
#include "Server.hpp"

/*============================================================================*/
                    /*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

Server::Server(/* const std::vector<std::string> & data */)
  : _backLog(1024)
{
	setSocket();
}
/*----------------------------------------------------------------------------*/

Server::Server(const std::vector<std::string> &)
  : _backLog(1024)
{	}
/*----------------------------------------------------------------------------*/

Server::Server(const Server & ref)
  : _backLog(ref._backLog)
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### DESTRUCTORS ###*/
/*============================================================================*/

Server::~Server()
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                    /*### OVERLOAD OPERATOR ###*/
/*============================================================================*/

Server  & Server::operator=(const Server &)
{ return *this; }
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Server & ref)
{
	o	<< "_params :" << std::endl
		<< ref.getParams();
	return o;
}
/*----------------------------------------------------------------------------*/

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
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### GETTER ###*/
/*============================================================================*/

/*
	conversion d'un non constant en constant pour eviter les betises
*/
t_paramServer	& Server::getParams() const
{ return const_cast<t_paramServer&>(_params); }
/*----------------------------------------------------------------------------*/

std::set<int>	& Server::getFdSet() const
{ return const_cast<std::set<int>&>(_fdSet); }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### SETTERS ###*/
/*============================================================================*/

/*
	initialise la variable _params
*/
void	Server::setParams(std::vector<std::string> & token)
{
	/* code */
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PRIVATE METHODS ###*/
/*============================================================================*/
#include <arpa/inet.h>
void	Server::setSocket() // voir wireshark
{
	struct addrinfo			hints, *res, *nextNode;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	for (std::set<std::string>::const_iterator it = _params.params["listen"].begin(); \
											it != _params.params["listen"].end(); it++)
	{
		int ret = getaddrinfo(NULL, it->c_str(), &hints, &res);
		if (ret != 0) {
			std::cerr	<< gai_strerror(ret) << " on port [ " << *it << " ]"
						<< std::endl;
			closeFdSet();
			throw std::runtime_error("Error -> getaddrinfo()");
		}
		for (nextNode = res; nextNode != NULL; nextNode = nextNode->ai_next)
		{
			int fd = socket(nextNode->ai_family, nextNode->ai_socktype | SOCK_NONBLOCK, nextNode->ai_protocol);
			if (fd < 0) {
				freeaddrinfo(res);
				freeAllServer(__FILE__, __LINE__, "Error -> socket()"); // continuer init ?
				throw std::runtime_error("");
			}
			
			
			try {
				setSockOptSafe(fd, res);
			} catch(const InitException& e) {
				e.setOptionExcept(fd, res);
				closeFdSet();
				throw;
			}
			

			{ // bloc test
				char buffer[1024];
				if (nextNode->ai_family == AF_INET) { // Adresse IPv4
					// il faut caster l'adresse en structure sockaddr_in pour récupérer
					// l'adresse IP, comme le champ ai_addr pourrait être soit
					// un sockaddr_in soit un sockaddr_in6
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)nextNode->ai_addr;
					// Transforme l'entier en adresse IP lisible
					inet_ntop(nextNode->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
					std::cout << "IPv4: " << buffer << std::endl;;
				} else { // Adresse IPv6
					struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)nextNode->ai_addr;
					inet_ntop(nextNode->ai_family, &(ipv6->sin6_addr), buffer, sizeof buffer);
					std::cout << "IPv6: " << buffer << std::endl;;
				}
			}

			// lie le socket a un port et une adresse
			if (bind(fd, nextNode->ai_addr, nextNode->ai_addrlen) != 0) {
				freeaddrinfo(res);
				freeAllServer(__FILE__, __LINE__, "Error -> bind()");
				std::cout   << "port : " << *it << std::endl;
				std::cout   << "fd : " << fd << std::endl;
				throw std::runtime_error("");
			}

			// socket en mode écoute pour les connexions entrantes,
			// spécifiant le nombre maximum de connexions en attente.
			if (listen(fd, 10) != 0) {
				freeaddrinfo(res);
				freeAllServer(__FILE__, __LINE__, "Error -> listen()");
				throw std::runtime_error("");
			}
			std::cout   << "fd : " << fd << std::endl;
			std::cout   << "port : " << *it << std::endl;
			
			_fdSet.insert(fd);
		}
		freeaddrinfo(res);
	}
}
/*----------------------------------------------------------------------------*/

void	Server::setSockOptSafe(int fd, struct addrinfo * res)
{
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0) {
		throw InitException(__FILE__, __LINE__, "Error -> setsockopt()");
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__, "Error -> setsockopt()");
	}
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__, "Error -> setsockopt()");
	}
}
/*----------------------------------------------------------------------------*/

void	Server::closeFdSet() const
{
	for (std::set<int>::iterator it = _fdSet.begin(); \
		it != _fdSet.end(); it++)
	{
		if (close(*it))
			std::cerr << "Error close" << std::endl;
	}
}
/*----------------------------------------------------------------------------*/

void	Server::freeAllServer(const std::string &file, const int line, const std::string & msg) const
{
	std::cerr   << msg
				<< " at file : [" << file << "] line : [" << line << "]" << std::endl
				<< strerror(errno) 
				<< std::endl;
	closeFdSet();
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PUBLIC METHODS ###*/
/*============================================================================*/

/*
	PARSING METHODS
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

/*
	mettre une erreur si different de 9 en size en comptant 'server'
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

const char * Server::InitException::what() const throw() {
	return "_msg.c_str()";
}
/*----------------------------------------------------------------------------*/

void	Server::InitException::setOptionExcept(const int fd, const struct addrinfo * res)
const throw()
{
	close(fd);
	freeaddrinfo(const_cast<struct addrinfo *>(res));
	std::cerr	<< "Error at file [" <<  "] line [" <<  "]"
				<< std::endl;
}
/*----------------------------------------------------------------------------*/

