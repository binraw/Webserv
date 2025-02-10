


/*============================================================================*/
								/*### HEADERS ###*/
/*============================================================================*/
#include "Cluster.hpp"
#include "ARequest.hpp"
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
#include <fstream>

#define MAXEVENT	10
#define BUFFERSIZE	2048

#define HTTPTEST	"HTTP/1.1 200 OK\r\n" \
					"Content-Type: text/html\r\n" \
					"Content-Length: 208\r\n" \
					"\r\n" \
					"<http>" \
					"<h1>TITLE<h1>" \
					"Hello, World!" \
					"<form action=/submit method=POST>" \
    				"<input type=text name=nom>" \
    				"<button type=submit>OK</button>" \
					"</form>" \
					"<http>"

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
	// for (std::map<std::string, Server >::const_iterator it = getServersByPort().begin();
	// it != getServersByPort().end(); it++)
	// 	std::cout	<< "port [" << it->first.c_str() << "] associate with server:" << std::endl
	// 				<< it->second;

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
	std::cout	<< BOLD BRIGHT_YELLOW "\nINIT TERMINATED\n" RESET << std::endl
				<< "at line " << __LINE__ << " file " << __FILE__ << " \n"  << *this ;
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
	o	<< BOLD BLUE << "CLUSTER:" << std::endl
		<< RESET << BLUE << "_serversByService:"
		<< std::endl << RESET;
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

const std::map<std::string, Server > & Cluster::getServersByPort()	const {
	return _serversByService;
}
/*----------------------------------------------------------------------------*/

const HttpConfig & Cluster::getConfig() const {
	return _config;
}
/*----------------------------------------------------------------------------*/

void	Cluster::readData(const struct epoll_event &event)
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> readData() {\n"
				<< "ClientSocket [" RESET PURPLE << event.data.fd 
				<< BOLD BRIGHT_PURPLE "]" RESET
				<< std::endl;
#endif
	int 		bytes_received = BUFFERSIZE;
	char		buffer[BUFFERSIZE] = {'\0'};
	std::string	response;
	
	while (bytes_received == BUFFERSIZE)
	{
		bytes_received = recv(event.data.fd, buffer, BUFFERSIZE, MSG_DONTWAIT);
		if (bytes_received < 0) {
			perror("recv");
			break; // handle error
		}
		response += buffer;
	}
#ifdef TEST
	std::cout	<< BRIGHT_PURPLE BOLD "MSG_CLIENT[" RESET
				<< PURPLE << response
				<< BRIGHT_PURPLE BOLD "]MSG_END" RESET
				<< std::endl;
	// std::ofstream logFile("header.log", std::ios::app);
	// if (logFile) {
	// 	logFile << response;
	// }
	// else {
	// 	perror("ERROR LOG TEST");
	// }
#endif

	// est ce que le client existe deja ?
	// std::set<Server>::const_iterator		itServer;
	// std::map<int, Client>::const_iterator	itClient;

	// for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	// {
	// 	itClient = itServer->getClientList().find(event.data.fd);
	// 	if (itClient != itServer->getClientList().end())
	// 		break;
	// }
	// if (response.find("POST", 0) != std::string::npos)
	// 	std::cout << RED "POST" RESET << std::endl;
	// else if (response.find("GET", 0) != std::string::npos)
	// 	std::cout << RED "GET" RESET << std::endl;
	// else
	// 	std::cout << RED "OTHER" RESET << std::endl;

	// itServer != _servers.end() ? itClient->second.handlerequest : _createClient()

	/*	* apres cette boucle
		* parser la requete
	*/




	try {
		changeEventMod(false, event.data.fd);
	}
	catch(const RunException& e) {
		e.runExcept();
		closeConnexion(event);
		throw;
	}

}

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
	int			httpSize = response.size();//strlen(http_response);

	while (bytes_sended != httpSize)
	{
		ssize_t ret = send(event.data.fd, response.c_str(), response.length(), 0);
		if (ret < 0) {
			perror("send()");
			break;
		}
		bytes_sended += ret;
	} 
	
	try {
		changeEventMod(true, event.data.fd);
	}
	catch(const RunException& e) {
		e.runExcept();
		closeConnexion(event);
		throw;
	}
}

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
						if (events[i].events & (EPOLLHUP | EPOLLRDHUP))
							closeConnexion(events[i]);
						else if (events[i].events & EPOLLIN)
							readData(events[i]);
						else if (events[i].events & EPOLLOUT)
							sendData(events[i]);
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

void Cluster::setServersByPort()
{
	std::vector<std::string>::const_iterator	itServiceList;
	std::vector<ServerConfig>::const_iterator	itConfigServer;
	std::pair<std::map<std::string, Server>::iterator, bool>	result;

	for (itConfigServer = _config._serversConfig.begin();
		itConfigServer != _config._serversConfig.end(); itConfigServer++)
	{
		for (itServiceList = itConfigServer->_listenPort.begin();
			itServiceList != itConfigServer->_listenPort.end(); itServiceList++)
		{
			result = _serversByService.insert(std::make_pair(*itServiceList, Server(*itConfigServer)));
			if (!result.second) {
				std::cerr << "Port [" << *itServiceList << "] still handle by server "
						  << *(result.first->second.getNameList().begin()) << std::endl;
			}
		}
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
	
	for (std::map<std::string, Server>::iterator it = _serversByService.begin(); \
												it != _serversByService.end(); it++)
	{
		int				sockFd = 0;
		struct addrinfo	*res = NULL;
		try {
			safeGetAddr(*it, &res);
			createAndLinkSocketServer(*res, it->first, &sockFd);
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
		}
		if (res)
			freeaddrinfo(res);
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
void	Cluster::safeGetAddr(const std::pair<const std::string, Server> &server, struct addrinfo **res) const
{
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
	std::set<std::string>::iterator it = server.second.getNameList().begin();
	for (; it != server.second.getNameList().end(); it++)
	{
		int ret = getaddrinfo(it->c_str(), server.first.c_str(), &hints, res);
		if (ret != 0) {
			std::string errorMsg = "Error -> setsockopt() at " + *it + " on port [" + server.first + "]";
			throw InitException(__FILE__, __LINE__ - 2, errorMsg.c_str(), ret);
		}
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
