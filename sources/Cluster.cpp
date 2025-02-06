



/*============================================================================*/
						/*### HEADER FILES ###*/
/*============================================================================*/
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
#define MAXEVENT 10

int g_runserv = 0;

void hand(int, siginfo_t *, void *)
{
    g_runserv = 0;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/
Cluster::Cluster(const std::string &filename)
throw(InitException) : _configPath(filename)
{
	setParams();

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

/*============================================================================*/
						/*### DESTRUCTORS ###*/
/*============================================================================*/
Cluster::~Cluster() {
	_epollFd > 0 ? close(_epollFd) : _epollFd;
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
	// _listenList.insert("100000");
	_listenList.insert("http");
	// _listenList.insert("8002");
	// _listenList.insert("8002 ");
	// _listenList.insert("8002");
	_workerConnexion = 1024;
	_keepAliveTime = 65;
}
/*----------------------------------------------------------------------------*/

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

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/
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

/*	* open epoll & add the server sockets to the set 
	* new fd added in the set epoll setting details :
		-> EPOLLIN: only for input data
		-> Levels triggered (by default) will automaticly empty the communication buffer
*/
void	Cluster::setEpollFd()
throw(InitException)
{
#ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setEpollFd() {"
				<< RESET << std::endl;
#endif
	struct epoll_event ev;

	_epollFd = epoll_create(1);
	if (_epollFd < 0)
		throw InitException(__FILE__, __LINE__ - 2, "error creation epoll()", NULL, 0);
	for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, *it, &ev) == -1) {
			close(_epollFd);
			throw InitException(__FILE__, __LINE__ - 2, "epoll_ctl: EPOLL_CTL_ADD", NULL, 0);
		}
	}
#ifdef TEST
	for (std::set<int>::iterator it = _serverSockets.begin(); \
								it != _serverSockets.end(); it++)
		std::cout << "fd [" << *it << "] added in epollFd" << std::endl;
	std::cout	<< BOLD BLUE "}\n" RESET
				<< std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

void	Cluster::closeFdSet()
{
	for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
		if (*it > 0 && close(*it) != 0)
			perror("close()");
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/


void	Cluster::parseHeader(const std::string &response)
{
	std::cout	<< "RECEIVED FROM CLIENT:\n"
				<< response << std::endl;



}

void	Cluster::readData(const struct epoll_event &event)
{
	int 		bytes_received = -1;
	char		buffer[2048] = {'\0'};
	std::string	response;

	do {
		bytes_received = recv(event.data.fd, buffer, sizeof(buffer), 0);
		if (bytes_received < 0) {
			perror("recv");
			return;
		}
		response += buffer;
		if (bytes_received < static_cast<int>(sizeof(buffer))) {
			std::cout << "HERE : bytes_received = " << bytes_received << " sizeof(buffer) = " << sizeof(buffer) << std::endl;
			break;

		}
	} while (bytes_received > 0);
	/*	* apres cette boucle
		* une fonction pour analyser la requete et envoyer la reponse appropriee
	*/
	// writeData(event);
	parseHeader(response);
}

void	Cluster::writeData(const struct epoll_event &event)
{
	const char *http_response =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: 157\r\n"  
	"\r\n"
	"<html>"
	"<head><title>Form Example</title></head>"
	"<body>"
	"<h1>TITLE</h1>"
	"<form action=\"/cgi-bin/script.pl\" method=\"post\">"
	"  <label for=\"name\">Name:</label><br>"
	"  <input type=\"text\" id=\"name\" name=\"name\"><br>"
	"  <input type=\"submit\" value=\"Submit\">"
	"</form>"
	"</body>"
	"</html>";
	if (send(event.data.fd, http_response, strlen(http_response), 0) <= 0) 
	{
		std::cerr << "Erreur lors de l'envoi de la réponse" << std::endl;
		return;
	}
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, event.data.fd, NULL);
	close(event.data.fd);
}

void	Cluster::acceptConnexion(const struct epoll_event &event) // rob: je pense que c'est ici que je vais check la requete
{
	socklen_t		addrSize;
	struct sockaddr *addr = NULL;
	const int		clientSocket = accept(event.data.fd, addr, &addrSize);
#ifdef TEST
	static int i = 0;
	if (i++ % 8 == 0) {
		close(clientSocket);
		throw RunException(__FILE__, __LINE__ - 1, "Error accept():");
	}
#else
	if (clientSocket < 0)
		throw RunException(__FILE__, __LINE__ - 1, "Error accept():");
#endif
	
	int flags = fcntl(clientSocket, F_GETFL, 0);
	if (flags == -1) {
		if (close(clientSocket) == -1)
			; // crash serveur ?
		throw RunException(__FILE__, __LINE__, "Error fcntl():");
	}
   	flags = flags | O_NONBLOCK | O_CLOEXEC;
   	if (fcntl(clientSocket, F_SETFL, flags) == -1) {
		if (close(clientSocket) == -1)
			; // crash serveur ?
		throw RunException(__FILE__, __LINE__, "Error fcntl():");
	}

	struct epoll_event	ev;

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = clientSocket;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
		if (close(clientSocket) == -1)
			; // crash serveur ?
		throw RunException(__FILE__, __LINE__, "Error epoll_ctl():");
	}
}

/*	* epoll()
	*
*/
void	Cluster::runCluster()
throw()
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int 		n = 0;
	
	/*	* MAXEVENT
		* nb max d'evenement I/O qui peuvent etre rapportes simultanement
		* un evenement signifie qu'un fd surveille par epoll est pret pour une operation d'I/O
		* cela signifie que le fd : 
			-> a des données disponibles en lecture
			-> peut ecrire / envoyer des donnees sans bloquer
			-> recoit une connexion sur un socket serveur (nouveau client donc ouverture socket client)
			-> Déconnexion d'un client (fermeture d'un socket client)
	*/
	struct epoll_event	events[MAXEVENT];
	g_runserv = 1;
	while (g_runserv)
	{
		int nbEvents = epoll_wait(_epollFd, events, MAXEVENT, 1000);
		if (nbEvents == -1) {
			perror("epoll_wait");
			break;
		} else if (nbEvents > 0) {
			for (int i = 0; i < nbEvents; i++) {
				try {
					if (_serverSockets.count(events[i].data.fd) > 0)
						acceptConnexion(events[i]);
					else
					{
						if (events[i].events & EPOLLIN) {
							std::cout << BRIGHT_RED "READ" RESET << std::endl;
							readData(events[i]);
						}
						else if (events[i].events & EPOLLOUT) {
							std::cout << BRIGHT_GREEN "WRITE" RESET << std::endl;
							writeData(events[i]);
						}
					}
				}
				catch(const RunException& e) {
					e.runExcept();
				}
			}
		} else {
			std::cout	<< "\rWaiting on a connection" << dot[n == 3 ? n = 0 : n++]
						<< std::flush;
		}
	}
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/
void	Cluster::InitException::setSockExcept() const throw() {
	if (errno != 0)
		std::cerr << RED << strerror(errno) << ": ";
	if (retAddr != 0)
		std::cerr << RED << gai_strerror(retAddr) << ": ";
	std::cerr << YELLOW "at file [" << _file << "] line [" << _line << "]";
	if (_serviceName != 0)
		std::cerr << " (serviceName [" << _serviceName << "])";
	std::cerr << RESET << std::endl << std::endl;
}
/*----------------------------------------------------------------------------*/

void	Cluster::RunException::runExcept() const throw() {
	if (errno != 0)
		std::cerr << RED << strerror(errno) << ": ";
	std::cerr	<< YELLOW "at file [" << _file << "] line [" << _line << "]"
				<< RESET << std::endl << std::endl;
}
/*----------------------------------------------------------------------------*/
