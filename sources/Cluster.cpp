



/*============================================================================*/
						/*### HEADER FILES ###*/
/*============================================================================*/
#include "Cluster.hpp"


#include <sys/epoll.h>
#include <sys/types.h>

#include <csignal>

#define MAXEVENT 10


/*============================================================================*/
					/*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

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
	// std::cout	<< "RECEIVED FROM CLIENT:\n"
	// 			<< buffer << std::endl;
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


Cluster::Cluster(const std::string &filename)
throw(std::exception) : _configPath(filename)
{
	setParams();

	setAllSocket();

	try {
		setEpollFd();
	}
	catch(const InitException& e) {
		e.setSockExcept();
		closeFdSet();
		throw;
	}
	
	
	std::cout << "\nINIT TERMINATED\n" << std::endl;


	{

		// sockaddr_storage est une structure qui n'est pas associé à
		// une famille particulière. Cela nous permet de récupérer
		// une adresse IPv4 ou IPv6
		// struct sockaddr_storage	client_addr;
		// socklen_t				addr_size;

		// addr_size = sizeof(client_addr);

		// epoll()

		// fd_client = accept(fd_sock_server, (struct sockaddr *) &client_addr, &client_addr_size); // accepte la connexion et creer un nouveau socket
		// if (fd_client < 0) {
		//     std::cerr << "Error accept client" << std::endl;
		//     freeaddrinfo(res);
		// }

		// memset(buffer, '\0', sizeof(buffer));
		// bytes_received = recv(fd_client, buffer, sizeof(buffer) - 1, 0);
		
		// if (bytes_received < 0) 
		// {
		// 	std::cerr << "Erreur lors de la réception des données" << std::endl;
		// 	close(fd_client);
		// 	freeaddrinfo(res);
		// 	return (5);
		// }
		// const char *http_response =
		// "HTTP/1.1 200 OK\r\n"
		// "Content-Type: text/html\r\n"
		// "Content-Length: 13\r\n"
		// "\r\n"
		// "Hello, World!";

		// if (send(fd_client, http_response, strlen(http_response), 0) < 0) 
		// {
		// 	std::cerr << "Erreur lors de l'envoi de la réponse" << std::endl;
		// 	close(fd_client);
		// 	freeaddrinfo(res);
		// 	return (6);
		// }

		// std::cout << http_response << std::endl;

		// close(fd_client);
		// std::cout << "New client : " << client_addr.ss_family << std::endl;
		// close(fd_sock_server);
	}

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
	// _listenList.insert("8002");
	// _listenList.insert("8002 ");
	// _listenList.insert("8002");
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
	std::cout	<< BOLD BLUE << "Function -> setSocket() {"
				<< RESET << std::endl;
# endif
	struct addrinfo	hints, *res = NULL;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;					// Spécifie qu'on utilise des adresses IPv6. Avec AI_V4MAPPED, cela permettra aussi de supporter IPv4.
	hints.ai_socktype = SOCK_STREAM;			// Indique un socket pour une communication par flux de données (orienté connexion, comme TCP).
	hints.ai_protocol = IPPROTO_TCP;			// Définit le protocole de transport comme étant TCP (Transmission Control Protocol).
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED;	// AI_PASSIVE : retourne une adresse utilisable par bind() pour écouter sur toutes les interfaces locales.
												// AI_V4MAPPED : permet d'accepter des connexions IPv4 sous forme d'adresses IPv6 mappées (ex. ::ffff:192.168.1.1).
	
	for (std::set<std::string>::iterator it = _listenList.begin(); \
										it != _listenList.end(); it++)
	{
		try {
			safeGetAddr(it->c_str(), hints, &res);
		}
		catch(const InitException &e) {
			e.setSockExcept();
			goto jump;
		}
		for (struct addrinfo *nextNode = res; nextNode != NULL; nextNode = nextNode->ai_next)
		{
			int fd = -1;
			try {
				safeSetSocket(nextNode, fd);
				safeLinkSocket(fd, nextNode, it->c_str());
				_sockFds.insert(fd);
			}
			catch(const InitException &e) {
				fd > 0 ? close(fd) : fd;
				e.setSockExcept();
				e.what();
			}
		}
		freeaddrinfo(res);
		jump:
			continue;
	}
# ifdef TEST
	std::cout	<< *this << std::endl
				<< BOLD BLUE "}\n" RESET
				<< std::endl;
# endif
	if (_sockFds.empty() == true)
		throw InitException(NULL, 0, "\033[31mnone of the requested services are available\n", \
							NULL, 0);
}
/*----------------------------------------------------------------------------*/

void	Cluster::setEpollFd()
{
	struct epoll_event ev;

#ifdef TEST
	std::cout	<< BOLD BLUE << "Function -> setEpollFd() {"
				<< RESET << std::endl;
#endif
	_epollFd = epoll_create(1);
	if (_epollFd < 0)
		throw InitException(__FILE__, __LINE__, "error creation epoll()", NULL, 0);
	for (std::set<int>::iterator it = _sockFds.begin(); \
								it != _sockFds.end(); it++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, *it, &ev) == -1) {
			close(_epollFd);
			throw InitException(__FILE__, __LINE__, "epoll_ctl: EPOLL_CTL_ADD", NULL, 0);
		}
	}
#ifdef TEST
	for (std::set<int>::iterator it = _sockFds.begin(); \
								it != _sockFds.end(); it++)
		std::cout << "fd [" << *it << "] added in epollFd" << std::endl;
	std::cout	<< BOLD BLUE "}\n" RESET
				<< std::endl;
#endif
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeGetAddr(const char *serviceName, const struct addrinfo &hints, struct addrinfo **res) const
throw(InitException)
{
	int ret = getaddrinfo(NULL, serviceName, &hints, res);
	if (ret != 0)
		throw InitException(__FILE__, __LINE__ - 2, "Error -> setsockopt()", serviceName, ret);
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeSetSocket(const struct addrinfo *currNode, int &fd) const
throw(InitException)
{
	fd = socket(currNode->ai_family, currNode->ai_socktype | SOCK_NONBLOCK, currNode->ai_protocol);
	if (fd < 0) {		
		throw InitException(__FILE__, __LINE__ - 2, "Error -> socket()", NULL, 0);
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) != 0) // R T MANUAL
		throw InitException(__FILE__, __LINE__ - 1, "Error -> fcntl()", NULL, 0);

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> setsockopt()", NULL, 0);
	}
}
/*----------------------------------------------------------------------------*/

void	Cluster::safeLinkSocket(const int sockFd, const struct addrinfo * currNode, const char *currPort) const
throw(Cluster::InitException)
{
	if (bind(sockFd, currNode->ai_addr, currNode->ai_addrlen) != 0) {
		throw InitException(__FILE__, __LINE__ - 1, "Error -> bind()", currPort, 0);
	}
	if (listen(sockFd, SOMAXCONN ) != 0) {
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

void	Cluster::runCluster()
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int 		n = 0;
	
	struct epoll_event	events[MAXEVENT], ev; // voir la doc
	struct sockaddr		*addr = NULL;
	socklen_t			addr_size;
	g_runserv = 1;
	while (g_runserv)
	{
		int clientSocket = -1;
		int nbEvents = epoll_wait(_epollFd, events, MAXEVENT, 1000);
		if (nbEvents == -1) {
			perror("epoll_wait");
			goto close;
		}
		if (nbEvents == 0)
			goto flush;
		
		
		for (std::set<int>::iterator it = _sockFds.begin(); it != _sockFds.end(); it++) {
			for (int i = 0; i < nbEvents; i++) {
				if (events[i].data.fd == *it) {
					clientSocket = accept(events[i].data.fd, addr, &addr_size);
					goto next;
				}
			}
			if (it == _sockFds.end())
				goto flush;
		}
		next:
		if (clientSocket < 0) {
			perror("accept()");
			goto close;
		}


		ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
		ev.data.fd = clientSocket;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
			perror("epoll_ctl: clientSocket");
			close(clientSocket);
			goto close;
		} else {
			// std::cout << "\nAdded fd [" << clientSocket << "]" << std::endl;
			communicateWithClient(clientSocket);
			epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientSocket, &ev);
			close(clientSocket);
		}
		flush:
			std::cout	<< "\rWaiting on a connection" << dot[n == 3 ? n = 0 : n++]
						<< std::flush;
	}
	close:
	close(_epollFd);

}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
							/*### EXCEPTIONS ###*/
/*============================================================================*/

void	Cluster::InitException::setSockExcept() const throw() {
	if (errno != 0)
		std::cerr << RED << strerror(errno) << ": ";
	if (_ret != 0)
		std::cerr << RED << gai_strerror(_ret) << ": ";
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
