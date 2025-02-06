


/*----------------------------------------------------------------------------*/
						/*### HEADER FILES ###*/
/*----------------------------------------------------------------------------*/
#include "Cluster.hpp"

#include <cstring>
#include <cerrno>

#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

/*
	ressources provisoirs
*/
#include <csignal>
#define MAXEVENT	10
#define BUFFERSIZE	2048
#define HTTPTEST	"HTTP/1.1 200 OK\r\n" \
					"Content-Type: text/html\r\n" \
					"Content-Length: 38\r\n" \
					"\r\n" \
					"<http>" \
					"<h1>TITLE<h1>" \
					"Hello, World!" \
					"<http>"


int g_runServer = 0;

void hand(int, siginfo_t *, void *)
{
    g_runServer = 0;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*----------------------------------------------------------------------------*/
Cluster::Cluster(const std::string &filename)
throw(InitException) : _configPath(filename), _epollFd(-1)
{
	setParams(); // provisoir

	try {
		setServerSockets();
	}
	catch(const std::exception& e) {
		closeFdSet();
		throw;
	}
	if (_serverSockets.empty() == true) {
		throw InitException(NULL, 0, ENOSERVICE, NULL, 0);
	}
	try {
		setEpollFd();
	}
	catch(const InitException& e) {
		e.setSockExcept();
		closeFdSet();
		throw;
	}
#ifdef TEST
	std::cout << BOLD BLUE "\nINIT TERMINATED\n" RESET << std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & )
{	}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
							/*### DESTRUCTOR ###*/
/*----------------------------------------------------------------------------*/
Cluster::~Cluster() {
	_epollFd > 0 ? close(_epollFd) : _epollFd;
	closeFdSet();
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
						/*### OVERLOAD OPERATOR ###*/
/*----------------------------------------------------------------------------*/
Cluster & Cluster::operator=(const Cluster & ) {
	return *this;
}
/*----------------------------------------------------------------------------*/

std::ostream	& operator<<(std::ostream & o, const Cluster &ref)
{
	o	<< BOLD BLUE << "CLUSTER:" << std::endl
		<< "std::set<std::string>	_listenList:" << RESET
		<< BLUE << std::endl;
	for (std::set<std::string>::iterator it = ref.getListenList().begin();
										it != ref.getListenList().end(); it++)
		o << "[" << *it << "]" << std::endl;
	return o << RESET;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
                        	/*### GETTER ###*/
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
                        	/*### SETTERS ###*/
/*----------------------------------------------------------------------------*/

/*	* temporary functions
*/
void	Cluster::setParams()
{
	_listenList.insert("8000");
	// _listenList.insert("8001");
	_listenList.insert("8002");
	// _listenList.insert("100000");
	// _listenList.insert("http");
	// _listenList.insert("8002");
	// _listenList.insert("8002 ");
	// _listenList.insert("8002");
	_workerConnexion = 1024;
	_keepAliveTime = 65;
}

void	displayClient(const struct epoll_event & ev)
{
	std::cout	<< "EVENT NUMBER [" << ev.events << "]\n"
				<< "FD CLIENT [" << ev.data.fd << "]\n";
}
/*----------------------------------------------------------------------------*/

/*	* open sockets server and bind them
*/
void	Cluster::setServerSockets()
throw(InitException)
{
# ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setSocket() {"
				<< RESET << std::endl;
# endif
	struct addrinfo	*res = NULL;
	
	for (std::set<std::string>::iterator it = _listenList.begin(); \
										it != _listenList.end(); it++)
	{
		int	sockFd = 0;
		try {
			safeGetAddr(it->c_str(), &res);
			createAndLinkSocketServer(*res, *it, &sockFd);
		}
		catch(const InitException &e) {
			switch (sockFd) {
				case -1:
					e.setSockExcept();
					throw;
				case 0:
					e.setSockExcept();	e.what();
					goto skipFreeAddrInfo;
				default:
					if (close(sockFd) != 0) {
						freeaddrinfo(res);
						throw InitException(__FILE__, __LINE__ - 2, "Error -> close()", NULL, 0);
					}
					e.setSockExcept(); e.what();
					break;
			}
		}
		freeaddrinfo(res);
		skipFreeAddrInfo:
			continue;
	}
# ifdef TEST
	std::cout	<< *this << std::endl
				<< BOLD BLUE "}\n" RESET
				<< std::endl;
# endif
}
/*----------------------------------------------------------------------------*/

/*	* open epoll & add the server sockets to the set 
*/
void	Cluster::setEpollFd()
throw(InitException)
{
#ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setEpollFd() {"
				<< RESET << std::endl;
#endif

	_epollFd = epoll_create(1);
	if (_epollFd < 0)
		throw InitException(__FILE__, __LINE__ - 2, "error creation epoll()", NULL, 0);

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

/*----------------------------------------------------------------------------*/
						/*### PRIVATE METHODS ###*/
/*----------------------------------------------------------------------------*/

/*	* get an availble address on an avaible service (port)
*/
void	Cluster::safeGetAddr(const char *serviceName, struct addrinfo **res) const
throw(InitException)
{
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
	int ret = getaddrinfo(NULL, serviceName, &hints, res);
	if (ret != 0)
		throw InitException(__FILE__, __LINE__ - 2, "Error -> setsockopt()", serviceName, ret);
}
/*----------------------------------------------------------------------------*/

/*	* opening + settings options + link SOCKET SERVER
	* non-blocking
	* SO_REUSEADDR allows reusing the port immediately after server shutdown,
	* avoiding "Address already in use" errors
*/
void	Cluster::createAndLinkSocketServer(const struct addrinfo &res, const std::string & serviceName, int *sockfd)
throw(InitException)
{
	for (const struct addrinfo *currNode = &res; currNode != NULL; currNode = currNode->ai_next)
	{
		*sockfd = socket(currNode->ai_family, currNode->ai_socktype | SOCK_NONBLOCK, currNode->ai_protocol);
		if (*sockfd < 0) {		
			throw InitException(__FILE__, __LINE__ - 2, "Error -> socket()", NULL, 0);
		}
		int opt = 1;
		if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
			throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
		}
		if (bind(*sockfd, currNode->ai_addr, currNode->ai_addrlen) != 0) {
			throw InitException(__FILE__, __LINE__ - 1, "Error -> bind()", serviceName.c_str(), 0);
		}
		if (listen(*sockfd, SOMAXCONN) != 0) {
			throw InitException(__FILE__, __LINE__ - 1, "Error -> listen()", serviceName.c_str(), 0);
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
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> acceptConnexion()\n" RESET
				<< PURPLE "ClientSocket [" << clientSocket << "]" RESET
				<< std::endl;
#endif

	int flags = fcntl(clientSocket, F_GETFL);
	if (flags == -1)
	{
		if (close(clientSocket) == -1)
			perror("close() in acceptConnexion()");
		throw RunException(__FILE__, __LINE__ - 2, "Error fcntl():");
	}
	flags |= O_NONBLOCK | O_CLOEXEC;
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
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> closeConnexion()\n" RESET
				<< PURPLE "Client fd [" << event.data.fd << "]" RESET
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
throw(RunException)
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
throw(RunException)
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
						/*### PUBLIC METHODS ###*/
/*----------------------------------------------------------------------------*/

void	Cluster::readData(const struct epoll_event &event)
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> readData() {" RESET
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
#endif
	/*	* apres cette boucle
		* creer une instance de serveur ou appeler un serveur d'un client deja existant
		* donner en parametre constructeur std::string response
		* les les client doivent avoir leur instance dediee
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

void	Cluster::writeData(const struct epoll_event &event)
{
#ifdef TEST
	std::cout	<< BOLD BRIGHT_PURPLE "\nFunction -> writeData()" RESET
				<< std::endl;
#endif
	const char *http_response = HTTPTEST;
	ssize_t		bytes_sended = 0;
	int			httpSize = strlen(http_response);

	while (bytes_sended != httpSize)
	{
		ssize_t ret = send(event.data.fd, http_response, strlen(http_response), 0);
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
		int nbEvents = epoll_wait(_epollFd, events, MAXEVENT, 1000);
		if (nbEvents == -1)
			perror("epoll_wait");
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
							writeData(events[i]);
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

/*----------------------------------------------------------------------------*/
							/*### EXCEPTIONS ###*/
/*----------------------------------------------------------------------------*/
void	Cluster::InitException::setSockExcept() const throw() {
	if (errno != 0)
		std::cerr << RED << strerror(errno) << ": ";
	if (retAddr != 0)
		std::cerr << RED << gai_strerror(retAddr) << ": ";
	std::cerr << YELLOW "at file [" << _file << "] line [" << _line << "]";
	if (_serviceName != 0)
		std::cerr << " (serviceName [" << _serviceName << "])";
	std::cerr << RESET << std::endl;
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
