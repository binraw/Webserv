



/*============================================================================*/
						/*### HEADER FILES ###*/
/*============================================================================*/
#include "Cluster.hpp"

/*============================================================================*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

Cluster::Cluster(const std::string &filename)
throw(std::exception) : _configPath(filename)
{
	setParams();
	setAllSocket();
	// sockaddr_storage est une structure qui n'est pas associé à
    // une famille particulière. Cela nous permet de récupérer
    // une adresse IPv4 ou IPv6
    // struct sockaddr_storage	client_addr;
    // socklen_t				addr_size;
}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & )
{	}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### DESTRUCTORS ###*/
/*============================================================================*/

Cluster::~Cluster() {
	closeFdSet();
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
					/*### OVERLOAD OPERATOR ###*/
/*============================================================================*/
Cluster & Cluster::operator=(const Cluster & ) {
	return *this;
}
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Cluster &ref)
{
	o	<< BOLD PURPLE << "CLUSTER:" << std::endl
		<< "std::set<std::string>	_listenList:" << RESET
		<< PURPLE << std::endl;
	for (std::set<std::string>::iterator it = ref.getListenList().begin();
										it != ref.getListenList().end(); it++)
		o << "[" << *it << "]" << std::endl;
	return o << RESET;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### GETTER ###*/
/*============================================================================*/
const std::string	& Cluster::getFileConfig() const {
	return const_cast<std::string &>(_configPath);
}
/*----------------------------------------------------------------------------*/

const std::vector<Server>	& Cluster::getAllServer() const {
	return const_cast<std::vector<Server> & >(_servers);
}
/*----------------------------------------------------------------------------*/

const std::set<std::string>	& Cluster::getListenList() const {
	return const_cast<std::set<std::string>	&>(_listenList);
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        	/*### SETTERS ###*/
/*============================================================================*/

void	Cluster::setParams()
{
	_listenList.insert("8000");
	_listenList.insert("8001");
	_listenList.insert("8002");
	_listenList.insert("8002");
	_listenList.insert("http");
	_listenList.insert("8002");
	_listenList.insert("8002 ");
	_listenList.insert("8002");
	_workerConnexion = 1024;
	_keepAliveTime = 65;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

void	Cluster::setAllSocket()
{
# ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setSocket()"
				<< RESET << std::endl;
# endif
	struct addrinfo	hints, *res = NULL;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
	
	for (std::set<std::string>::iterator it = _listenList.begin(); it != _listenList.end(); it++)
	{
		try {
			safeGetAddr(it->c_str(), hints, &res);
		}
		catch(const InitException &e) {
			e.getAddrExcept();
			goto jump0;
		}
		for (struct addrinfo *nextNode = res; nextNode != NULL; nextNode = nextNode->ai_next)
		{
			int fd = -1;
			try {
				safeSetSocket(nextNode, fd);
				safeLinkSocket(fd, nextNode, it->c_str());
			}
			catch(const InitException &e) {
				fd > 0 ? close(fd) : fd;
				e.setSockExcept();
				e.what();
				goto jump1;
			}
			_sockFds.insert(fd);
			jump1:
				continue;
		}
		freeaddrinfo(res);
		jump0:
			continue;
	}
# ifdef TEST
	std::cout	<< *this << std::endl;
# endif
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeGetAddr(const char *name, const struct addrinfo &hints, struct addrinfo **res) const
throw(InitException)
{
	int ret = getaddrinfo(NULL, name, &hints, res);
	if (ret != 0)
		throw InitException(__FILE__, __LINE__ - 2, "Error -> setsockopt()", name, ret);
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeSetSocket(const struct addrinfo *currNode, int &fd) const
throw(InitException)
{
	fd = socket(currNode->ai_family, currNode->ai_socktype | SOCK_NONBLOCK, currNode->ai_protocol);
	if (fd < 0) {		
		throw InitException(__FILE__, __LINE__ - 2, "Error -> socket()", NULL, 0);
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) != 0) // R T MANUAL
		throw InitException(__FILE__, __LINE__ - 1, "Error -> fcntl()", NULL, 0);

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
	}
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeLinkSocket(const int sockFd, const struct addrinfo * currNode, const char *currPort) const
throw(InitException)
{
	if (bind(sockFd, currNode->ai_addr, currNode->ai_addrlen) != 0) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> bind()", currPort, 0);
	}
	// socket en mode écoute pour les connexions entrantes,
	// spécifiant le nombre maximum de connexions en attente BACKLOG.
	if (listen(sockFd, DFLT_BACKLOG ) != 0) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> listen()", currPort, 0);
	}
}
/*----------------------------------------------------------------------------*/

void	Cluster::closeFdSet()
{
	for (std::set<int>::iterator it = _sockFds.begin(); it != _sockFds.end(); it++)
		if (*it > 0 && close(*it) != 0)
			std::cerr	<< RED "Error when closing fd " << *it
						<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/

void	Cluster::InitException::setSockExcept() const throw() {
	std::cerr	<< RED << strerror(errno)
				<< std::endl
				<< YELLOW "at file [" << _file << "] line [" << _line << "]";
	if (_serviceName != NULL) {
		std::cerr	<< " ( port [" << _serviceName << "] )";
	}
	std::cerr	<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

void	Cluster::InitException::getAddrExcept() const throw() {
	std::cerr	<< RED << gai_strerror(_ret) << std::endl
				<< YELLOW "function getaddrinfo() " __FILE__ " at line [" << __LINE__ - 3 << "] : "
				<< "The service [" << _serviceName << "] could not be launched."
				<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

const char *	Cluster::InitException::what() const throw() {
	return _msg;
}
/*----------------------------------------------------------------------------*/
