

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"
#include "HttpConfig.hpp"

#include "Server.hpp"

/*	* MIME LIST
	// typedef struct s_paramsServer
	// {
	// 	std::map<std::string, std::string>
	// 		mimes;
	// 	s_paramsServer() 
	// 	{
	// 		mimes[".bin"].assign("application/octet-stream");
	// 		mimes[".js"].assign("application/javascript");
	// 		mimes[".doc"].assign("application/msword");
	// 		mimes[".csh"].assign("application/x-csh");
	// 		mimes[".json"].assign("application/json");
	// 		mimes[".pdf"].assign("application/pdf");
	// 		mimes[".sh"].assign("application/x-sh");
	// 		mimes[".jpeg"].assign("image/jpeg");
	// 		mimes[".jpg"].assign("image/jpeg");
	// 		mimes[".bmp"].assign("image/bmp");
	// 		mimes[".gif"].assign("image/gif");
	// 		mimes[".png"].assign("image/png");
	// 		mimes[".html"].assign("text/html");
	// 		mimes[".htm"].assign("text/html");
	// 		mimes[".css"].assign("text/css");
	// 		mimes[".csv"].assign("text/csv");

	// 	}

	// }	t_paramsCluster;
*/

class Cluster
{
	class   InitException : virtual public std::exception
	{
		public:
			InitException(const char *file, int line, const char *msg, const char *serviceName, const int ret)
				: retAddr(ret), _file(file), _line(line), _msg(msg), _serviceName(serviceName)
			{	}
			const char *	what()			const throw() {
				return _msg;
			};
			void			setSockExcept() const throw();
			const int 		retAddr;

		private:
			const char *	_file;
			const int 		_line;
			const char *	_msg;
			const char *	_serviceName;	
	};

	class   RunException : virtual public std::exception
	{
		public:
			RunException(const char *file, int line, const char *msg)
				: _file(file), _line(line), _msg(msg)
			{	}
			const char *	what() const throw() {
				return _msg;
			};
			void			runExcept() const throw();

		private:
			const char *	_file;
			const int 		_line;
			const char *	_msg;
	};

	public:
		Cluster(const std::string &) throw(InitException);
		Cluster(const Cluster &);
		~Cluster();
		Cluster & operator=(const Cluster &);

		const std::string			&getFileConfig()	const;
		const std::vector<Server>	&getAllServer()	const;
		const std::set<std::string>	&getServiceList()	const;
		const std::set<Server>		&getServers()	const;
		const HttpConfig			&getConfig()	const;

		void	runCluster();

		void	writeData(const struct epoll_event &);
		void	readData(const struct epoll_event &);

	private:
		int				_epollFd;		// fd vers structure epoll
		std::set<int>	_serverSockets;	// ensemble des socket serveur (un par port)
		const HttpConfig	_config;
		std::set<Server>	_servers;

		int				_epollFd;			// fd vers structure epoll
		std::set<int>	_serverSockets;		// ensemble des socket serveur
		const HttpConfig		_config;
		std::set<std::string>	_serviceList;

		void	setServers();
		void	setServiceList();
		void	setEpollFd();
		void	setServerSockets();
		void	safeGetAddr(const char *, struct addrinfo **) const;
		void	createAndLinkSocketServer(const struct addrinfo &, const std::string &, int *);
		void	closeFdSet() const;

		void	acceptConnexion(const struct epoll_event &) const;
		void	closeConnexion(const struct epoll_event &event) const;
		void	addFdInEpoll(const bool, const int)	const throw(RunException);
		void	changeEventMod(const bool, const int) const throw(RunException);
};

std::ostream	& operator<<(std::ostream &, const Cluster &);

#endif
