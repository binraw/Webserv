

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "../includes/webserv.hpp"

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
			const char *	what()			const throw();
			void			setSockExcept() const throw();
			const int 		retAddr;

		private:
			const char *	_file;
			const int 		_line;
			const char *	_msg;
			const char *	_serviceName;	
	};

	public:
		Cluster(const std::string &filename) throw(InitException);
		Cluster(const Cluster &);
		~Cluster();
		Cluster &	operator=(const Cluster &);

		const std::string			& getFileConfig()	const;
		const std::vector<Server>	& getAllServer()	const;
		const std::set<std::string>	& getListenList()	const;

		void	runCluster();

	private:
		std::set<std::string>	_listenList;		// liste de tous les ports
		std::set<std::string>	_incudeList;		// ??
		std::vector<Server>		_servers;			// ensemble des servers present dans le cluster
		std::set<int>			_serverSockets;		// ensemble des socket serveur
		std::string				_configPath;		// chemin vers fichier de config
		std::string				_defaultType;		// pour default entete http
		int						_workerConnexion;	// nb total de connexion supportes par le cluster
		int						_keepAliveTime;		// le temps que le serveur garde une conneion active entre deyux requetes (secondes)
		int						_epollFd;			// fd vers structure epoll

		void	setParams();		// init les parametres (provisoir en attendant parsing)

		void	setEpollFd() throw(InitException);
		void	setServerSockets() throw(InitException);
		void	safeGetAddr(const char *, struct addrinfo **) const throw(InitException); 
		void	createAndLinkSocketServer(const struct addrinfo &, const std::string &, int *) throw(InitException);

		void	handleConnexion(const struct epoll_event &event);

		void	closeFdSet();
};

std::ostream	& operator<<(std::ostream &, const Cluster &);

#endif
