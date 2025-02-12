

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


// timeout client ne fonctionne pas correctement

class Cluster
{
	class   InitException : virtual public std::exception
	{
		public:
			InitException(const char *file, int line, const std::string &msg, const int ret)
				: retAddr(ret), _file(file), _line(line), _msg(msg)
			{	}
			virtual ~InitException() throw() {}
			virtual const char *	what() const throw() {
				return _msg.c_str();
			};
			void			setSockExcept() const throw();
			const int 		retAddr;

		private:
			const char *_file;
			const int 	_line;
			std::string	_msg;
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
		Cluster(const std::string &);
		Cluster(const Cluster &);
		~Cluster();
		Cluster & operator=(const Cluster &);

		const std::map<std::string, Server >	&getServersByPort()	const;
		const HttpConfig			&getConfig()	const;

		void	runCluster();

		void	sendData(const struct epoll_event &);
		void	readData(const struct epoll_event &);

	private:
		HttpConfig		_config;
		int				_epollFd;		// fd vers structure epoll
		std::set<int>	_serverSockets;	// ensemble des socket serveur (un par port)
		std::map<std::string, Server >	_serversByService;

		void	setServersByPort();
		void	setEpollFd();
		void	setServerSockets();
		
		void	safeGetAddr(const char *, struct addrinfo **) const;
		void	createAndLinkSocketServer(const struct addrinfo &, const std::string &, int *);
		void	closeFdSet() const;

		void	acceptConnexion(const struct epoll_event &) const;
		void	closeConnexion(const struct epoll_event &event) const;
		void	addFdInEpoll(const bool, const int)	const;
		void	changeEventMod(const bool, const int) const;
};

std::ostream	& operator<<(std::ostream &, const Cluster &);

#endif
