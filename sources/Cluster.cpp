


/*============================================================================*/
								/*### HEADERS ###*/
/*============================================================================*/
#include "Cluster.hpp"
#include "ConfigParser.hpp"

#include <cstring>
#include <cerrno>

#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>


/*	* ressources provisoirs
*/
#include <csignal>

#define MAXEVENT	10
#define BUFFERSIZE	2048

int g_runServer = 0;

void hand(int, siginfo_t *, void *)
{
    g_runServer = 0;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/

Cluster::Cluster(const std::string &filepath)
  : _config(ConfigParser().parse(filepath))
{
	setServersByPort();
	try {
		_serverSockets.clear();
		setServerSockets();
	}
	catch(const std::exception& e) {
		closeFdSet();
		throw;
	}

	if (_serverSockets.empty() == true) 
		throw InitException(NULL, 0, ENOSERVICE, 0);

	try {
		_epollFd = -1;
		setEpollFd();
	}
	catch(const InitException& e) {
		e.setSockExcept();
		closeFdSet();
		throw;
	}
#ifdef TEST
	std::cout	<< std::endl << *this
				<< BOLD BRIGHT_YELLOW "\nINIT TERMINATED\n" RESET
				<< std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & )
{	}
/*----------------------------------------------------------------------------*/

Cluster::~Cluster() {
	_epollFd > 0 ? close(_epollFd) : _epollFd;
	closeFdSet();
}
/*----------------------------------------------------------------------------*/

Cluster & Cluster::operator=(const Cluster & ) {
	return *this;
}
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Cluster &ref)
{
	o	<< BOLD "CLUSTER:" RESET << std::endl
		<< "_serversByService:"
		<< std::endl;
	for (std::map<std::string, Server >::const_iterator it = ref.getServersByPort().begin();
		it != ref.getServersByPort().end(); it++)
			o	<< "port [" + it->first + "] associate with server:" << std::endl
				<< it->second << std::endl;
	return o << RESET;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/

std::map<std::string, Server> & Cluster::getServersByPort() const {
	return const_cast<std::map<std::string, Server> & >( _serversByService);
}
/*----------------------------------------------------------------------------*/

const HttpConfig & Cluster::getConfig() const {
	return _config;
}
/*----------------------------------------------------------------------------*/

/*	* server main loop
	* MAXEVENT
		. nb max d'evenement I/O qui peuvent etre rapportes simultanement
		. un evenement signifie qu'un fd surveille par epoll est pret pour une operation d'I/O
		. cela signifie que le fd : 
			-> a des données disponibles en lecture
			-> peut ecrire / envoyer des donnees sans bloquer
			-> recoit une connexion sur un socket serveur (nouveau client donc ouverture socket client)
			-> Déconnexion d'un client (fermeture d'un socket client)
*/
void	Cluster::runCluster()
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int 		n = 0;
	
	struct epoll_event	events[MAXEVENT];
	g_runServer = 1;
	while (g_runServer)
	{
		int nbEvents = epoll_wait(_epollFd, events, MAXEVENT, 777);
		if (nbEvents == -1)
			perror("\nepoll_wait");
		else if (nbEvents > 0) {
			for (int i = 0; i < nbEvents; i++) {
				try {
					if (_serverSockets.count(events[i].data.fd) > 0)
						acceptConnexion(events[i]);
					else
					{
						if (events[i].events & EPOLLIN) {
							recvData(events[i]);
						}
						else if (events[i].events & EPOLLOUT) {
							sendData(events[i]);
						}
						else if (events[i].events & (EPOLLHUP | EPOLLRDHUP))
							closeConnexion(events[i]);
						else
							std::cerr << "have to print EPOLLERR" << std::endl;
					}
				}
				catch(const std::exception& e) {
					std::cerr << e.what() << std::endl;
				}
			}
		}
		else
			std::cout	<< "\rWaiting on a connection" << dot[n == 3 ? n = 0 : n++]
						<< std::flush;
	}
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

/*	* Create a new client on the matching server
	* init the client with the parsed request
	* assign a pointer to the server associated with the request
*/
Client * Cluster::addClient(const Request &req, const int fdClient)
{
	Server *current = NULL;
	try {
		current = &getServersByPort().at(req.gethostport());
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << RED " Error\nno matching server\n" RESET;
		return NULL;
	}

	try {
		std::pair<std::map<int, Client>::iterator, bool> it;
		it = current->getClientList().insert(std::pair<int, Client>(fdClient, Client(req)));
		it.first->second._clientServer = current;
		return &it.first->second;
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << RED " Error\nadding client to list\n" RESET;
		return NULL;
	}
	return NULL;
}
/*----------------------------------------------------------------------------*/

/*	* Search for a client and return a pointer to it
    * If no client is found, return a null pointer  
*/
Client *	Cluster::findClient(int fdClient)
{
	std::map<std::string, Server>::iterator itServer = _serversByService.begin();
	Client	*client = NULL;
	while (itServer != _serversByService.end())
	{
		try {
			client = &itServer->second.getClientList().at(fdClient);
			client->_clientServer = &itServer->second;
		}
		catch(const std::exception& e) {
			;
		}
		itServer++;
	}
	return client;
}
/*----------------------------------------------------------------------------*/

/*  * Receive data, parse the header, and extract the body of the request  
    * The while loop receives data  
    * If it's a new client, create one; if it already exists, update its request  
*/
void	Cluster::recvData(const struct epoll_event &event)
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> readData() {\n"
				<< "ClientSocket [" RESET PURPLE << event.data.fd 
				<< BOLD BRIGHT_PURPLE "]" RESET
				<< std::endl;
#endif

	int 		bytes_received = BUFFERSIZE;
	char		buffer[BUFFERSIZE] = {'\0'};
	std::string	buff("\0");
	
	while (bytes_received == BUFFERSIZE)
	{
		bytes_received = recv(event.data.fd, buffer, BUFFERSIZE, 0);
		if (bytes_received == 0)
			break; // client closed connexion
		if (bytes_received < 0) {
			perror("recv");
			break; // handle error
		}
		buff.append(buffer, bytes_received);
	}
	if (buff.size() == 0)
		return;
	Client *client = findClient(event.data.fd);
	if (!client)
	{
		try {
			client = addClient(Request(buff), event.data.fd);
		}
		catch(const std::exception& e) {
			std::cerr << e.what() << " " __FILE__ << " : " << __LINE__ << std::endl;
		}
	}
	else
	{
		if (client->getrequest().gettype().empty() == true)
		{
			try {
				client->getrequest() = Request(buff);
			}
			catch(const std::exception& e) {
				std::cerr << e.what() << " " __FILE__ << " : " << __LINE__ << std::endl;
			}		
		}
		else {
			if (client->getrequest().getbody().empty() == true)
				client->getrequest().getbody() = buff;
			else
				client->getrequest().getbody().append(buff, buff.size());
		}
	}

	if (client->getrequest().getcontentlength() == client->getrequest().getbody().size())
	{
		try {
			changeEventMod(false, event.data.fd);
		}
		catch(const RunException& e) {
			e.runExcept();
			closeConnexion(event);
			throw;
		}
	}
#ifdef TEST
	std::cout << client->getrequest() << std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

void	Cluster::sendData(const struct epoll_event &event)
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> sendData()\n"
				<< "ClientSocket [" RESET PURPLE << event.data.fd
				<< BOLD BRIGHT_PURPLE "]\n" RESET
				<< std::endl;
#endif
	char buff[4096];
	memset(buff, '\0', sizeof(buff));

	int fd = open("./website/form.html", O_RDONLY);
	if (fd == -1)
		perror("OPENTEST");	
	read(fd, buff, sizeof(buff));
	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 400\r\n\r\n";
	response += buff;

	ssize_t		bytes_sended = 0;
	int			httpSize = response.size();

	while (bytes_sended != httpSize)
	{
		ssize_t ret = send(event.data.fd, response.c_str(), response.length(), 0);
		if (ret < 0) {
			perror("send()");
			break;
		}
		bytes_sended += ret;
	} 
	
	Client *client = findClient(event.data.fd);
	client->getrequest().clearRequest();
	closeConnexion(event);

	// try {
	// 	changeEventMod(true, event.data.fd);
	// }
	// catch(const RunException& e) {
	// 	e.runExcept();
	// 	closeConnexion(event);
	// 	throw;
	// }
}
/*----------------------------------------------------------------------------*/

/*	* init servers from _serverconfig
	*
*/
void Cluster::setServersByPort()
{
	std::vector<ServerConfig>::iterator	itConfigServer = _config._serversConfig.begin();
	std::pair<std::map<std::string, Server>::iterator, bool>	result;

	while (itConfigServer != _config._serversConfig.end())
	{
		std::vector<std::string>::iterator	itServiceList = itConfigServer->_listenPort.begin();
		while (itServiceList != itConfigServer->_listenPort.end())
		{
			result = _serversByService.insert(std::make_pair(*itServiceList, Server(*itConfigServer, *itServiceList)));
			if (!result.second)
			{
				std::cerr << YELLOW "Port [" << *itServiceList << "] still required by server "
						  << *(result.first->second.getNameList().begin())
						  << RESET << std::endl;
			}
			itServiceList++;
		}
		itConfigServer++;
	}
}
/*----------------------------------------------------------------------------*/

/*	* open sockets server and bind them
*/
void	Cluster::setServerSockets()
{
# ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setServerSockets() {"
				<< RESET << std::endl;
# endif

	std::map<std::string, Server>::iterator itServer = _serversByService.begin();
	
	while (itServer != _serversByService.end())
	{
		int				sockFd = 0;
		struct addrinfo	*res = NULL;
		try {
			safeGetAddr(itServer->first.c_str(), &res);
			createAndLinkSocketServer(*res, itServer->first, &sockFd);
			freeaddrinfo(res);
			itServer++;
		}
		catch(const InitException &e) {
			if (sockFd == -1){
				e.setSockExcept();
				throw;
			}
			else if (sockFd > 0 && close(sockFd) != 0) {
				freeaddrinfo(res);
				throw InitException(__FILE__, __LINE__ - 2, "Error -> close()", 0);
			}
			e.setSockExcept();
			std::cerr << (e.what() != NULL ? e.what() : "") << std::endl;

			std::map<std::string, Server>::iterator tmp = itServer;
			tmp++;
			_serversByService.erase(itServer);
			itServer = tmp;
		}
	}
# ifdef TEST
	std::cout	<< BOLD BLUE "}\n" RESET
				<< std::endl;
# endif
}
/*----------------------------------------------------------------------------*/

/*	* open epoll & add the server sockets to the set 
*/
void	Cluster::setEpollFd()
{
#ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setEpollFd() {"
				<< RESET << std::endl;
#endif

	_epollFd = epoll_create(1);
	if (_epollFd < 0)
		throw InitException(__FILE__, __LINE__ - 2, "error creation epoll()", 0);

	for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
		addFdInEpoll(true, *it);

#ifdef TEST
	for (std::set<int>::iterator it = _serverSockets.begin(); \
								it != _serverSockets.end(); it++)
		std::cout << BLUE "fd [" << *it << "] added in epollFd" << std::endl;
	std::cout	<< BOLD BLUE "}\n" RESET
				<< std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

/*	* get an availble address on an avaible service (port)
*/
void	Cluster::safeGetAddr(const char *serviceName, struct addrinfo **res) const
{
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
	int ret = getaddrinfo(NULL, serviceName, &hints, res);
	if (ret != 0) {
		std::string errorMsg = "Error -> safeGetAddr() on port [" + std::string(serviceName) + "]";
		throw InitException(__FILE__, __LINE__ - 2, errorMsg.c_str(), ret);
	}
}
/*----------------------------------------------------------------------------*/

/*	* opening + settings options + link SOCKET SERVER
	* non-blocking
	* SO_REUSEADDR allows reusing the port immediately after server shutdown,
	* avoiding "Address already in use" errors
*/
void	Cluster::createAndLinkSocketServer(const struct addrinfo &res, const std::string & serviceName, int *sockfd)
{
	for (const struct addrinfo *currNode = &res; currNode != NULL; currNode = currNode->ai_next)
	{
		*sockfd = socket(currNode->ai_family, currNode->ai_socktype | SOCK_NONBLOCK, currNode->ai_protocol);
		if (*sockfd < 0) {		
			throw InitException(__FILE__, __LINE__ - 2, "Error -> socket()", 0);
		}
		int opt = 1;
		if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
			throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", 0);
		}
		if (bind(*sockfd, currNode->ai_addr, currNode->ai_addrlen) != 0) {
			throw InitException(__FILE__, __LINE__ - 1, std::string("Error -> bind() on port [" + serviceName + "]").c_str(), 0);
		}
		if (listen(*sockfd, SOMAXCONN) != 0) {
			throw InitException(__FILE__, __LINE__ - 1, std::string("Error -> listen() on port [" + serviceName + "]").c_str(), 0);
		}
		_serverSockets.insert(*sockfd);
	}
}
/*----------------------------------------------------------------------------*/

/*	* accept a new client connexion, set the socket and add the fd in the epoll set
	*
*/
void	Cluster::acceptConnexion(const struct epoll_event &event) const
{
	struct sockaddr addr;
	socklen_t		addrSize = sizeof(addr);
	const int		clientSocket = accept(event.data.fd, &addr, &addrSize);

	if (clientSocket < 0)
		throw RunException(__FILE__, __LINE__ - 3, "Error accept():");

#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> acceptConnexion()\n"
				<< "ClientSocket [" RESET PURPLE << clientSocket
				<< BOLD BRIGHT_PURPLE "]" RESET
				<< std::endl;
#endif

	int flags = O_NONBLOCK | FD_CLOEXEC;
   	if (fcntl(clientSocket, F_SETFL, flags) == -1)
	{
		if (close(clientSocket) == -1)
			perror("close() in acceptConnexion()");
		throw RunException(__FILE__, __LINE__ - 2, "Error fcntl():");
	}
	try {
		addFdInEpoll(false, clientSocket);
	}
	catch(const RunException& e) {
		e.runExcept();
		closeConnexion(event);
		throw;
	}
}
/*----------------------------------------------------------------------------*/

/*	* close a client connexion
*/
void	Cluster::closeConnexion(const struct epoll_event &event) const
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> closeConnexion()\n"
				<< "Client fd [" RESET PURPLE << event.data.fd
				<< BOLD BRIGHT_PURPLE "]" RESET
				<< std::endl;
#endif
	
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, event.data.fd, NULL) == -1)
		perror("epoll_ctl() in closeConnexion()");

	if (close(event.data.fd) == -1)
		perror("close() in closeConnexion()");
}
/*----------------------------------------------------------------------------*/

/*	* init epoll events for servers & new client socket and add the fd in epoll set
*/
void	Cluster::addFdInEpoll(const bool isServerSocket, const int fd) const
{
	struct epoll_event	ev;
	memset(&ev, 0, sizeof(ev));

	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
	
	if (isServerSocket == false)
		ev.events |= EPOLLET;
	
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw RunException(__FILE__, __LINE__ - 1, "Error epoll_ctl() in addFdInEpoll() :");
}
/*----------------------------------------------------------------------------*/

/*	* switch events mode between EPOLLOUT and EPOLLIN
*/
void	Cluster::changeEventMod(const bool changeForRead, const int fd) const
{
	struct epoll_event	ev;
	memset(&ev, 0, sizeof(ev));
	
	ev.data.fd = fd;
	ev.events |= EPOLLET | EPOLLHUP | EPOLLRDHUP | (changeForRead ? EPOLLIN : EPOLLOUT);
	
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw RunException(__FILE__, __LINE__ - 1, "Error epoll_ctl() in changeEventMod():");
}
/*----------------------------------------------------------------------------*/

/*	* close all server sockets
*/
void	Cluster::closeFdSet() const
{
	for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
		if (*it > 0 && close(*it) != 0)
			perror("close()");
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
							/*### EXCEPTIONS ###*/
/*----------------------------------------------------------------------------*/
void	Cluster::InitException::setSockExcept() const throw() {
	if (errno != 0)
		std::cerr << RED << strerror(errno) << ": ";
	if (retAddr != 0)
		std::cerr << RED << gai_strerror(retAddr) << ": ";
	
	std::cerr	<< YELLOW "at file [" << _file << "] line [" << _line << "]"
				<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/

void	Cluster::RunException::runExcept() const throw() {
	std::cerr << RED "Error" << std::endl;
	if (errno != 0)
		std::cerr << strerror(errno) << ": ";
	std::cerr	<< YELLOW "at file [" << _file << "] line [" << _line << "]"
				<< RESET << std::endl;
}
/*----------------------------------------------------------------------------*/
