



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

int	communicateWithClient(const int clientFd)
{
	char buffer[2048];
	memset(buffer, '\0', sizeof(buffer));
	int bytes_received = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	
	if (bytes_received < 0) 
	{
		std::cerr << "Erreur lors de la réception des données" << std::endl;
		return (5);
	}
	std::cout	<< "RECEIVED FROM CLIENT:\n"
				<< buffer << std::endl;
	const char *http_response =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: 38\r\n"
	"\r\n"
	"<http>"
	"<h1>TITLE<h1>"
	"Hello, World!"
	"<http>";

	if (send(clientFd, http_response, strlen(http_response), 0) < 0) 
	{
		std::cerr << "Erreur lors de l'envoi de la réponse" << std::endl;
		return (6);
	}
	return (0);
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
	close(_epollFd);
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
		throw InitException(__FILE__, __LINE__, "error creation epoll()", NULL, 0);
	for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, *it, &ev) == -1) {
			close(_epollFd);
			throw InitException(__FILE__, __LINE__, "epoll_ctl: EPOLL_CTL_ADD", NULL, 0);
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

void	Cluster::handleConnexion(const struct epoll_event & event)
{
	int					clientSocket = -1;
	socklen_t			addrSize;
	struct sockaddr		*addr = NULL;
	struct epoll_event	ev;

	clientSocket = accept(event.data.fd, addr, &addrSize);
	if (clientSocket < 0) {
		perror("accept()");
		return;
	}
	std::cout << "EVENT : " << event.events << " ";

	ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
	ev.data.fd = clientSocket;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
		perror("epoll_ctl: clientSocket");
		close(clientSocket);
		return;
	}
	else {
		communicateWithClient(clientSocket);
		epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientSocket, &ev);
		close(clientSocket);
	}
}

/*
	Dans le contexte d'epoll, un événement représente une activité
	sur un descripteur de fichier surveillé.

	Plus précisément :
	Ce n'est pas le nombre max de clients, mais le nombre max d'événements
	d'I/O qui peuvent être rapportés simultanément
	Un événement signifie qu'un descripteur de fichier (socket) est prêt
	pour une opération d'I/O

	Cela peut indiquer :
	Des données disponibles en lecture
	Possibilité d'écriture sans bloquer
	Connexion entrante sur un socket serveur
	Déconnexion d'un client
*/
void	Cluster::runCluster()
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
		}
		if (nbEvents > 0) {
			// std::cout<< "\nevents treated nb events : " << nbEvents << std::endl;
			for (std::set<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++) {
				for (int i = 0; i < nbEvents; i++) {
					if (events[i].data.fd == *it) {
						handleConnexion(events[i]);
						// std::cout << "count : ";
					}
				}
			}
			// std::cout << std::endl;
		}
		std::cout	<< "\rWaiting on a connection" << dot[n == 3 ? n = 0 : n++]
					<< std::flush;
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

const char *	Cluster::InitException::what() const throw() {
	return _msg;
}
/*----------------------------------------------------------------------------*/
