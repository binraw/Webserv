#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "Server.hpp"

#include <sstream>
#include <fstream>
#include <cstring>
#include <cerrno>

# include <unistd.h>
# include <fcntl.h>
# include <netdb.h>


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
class Cluster
{
	class   InitException : virtual public std::exception
	{
		public:
			InitException(const char *file, int line, const char *msg, const char *serviceName, const int ret)
				: _file(file), _line(line), _ret(ret), _msg(msg), _serviceName(serviceName)
			{	}
			const char *	what()			const throw();
			void			setSockExcept() const throw();

		private:
			const char *	_file;
			const int 		_line;
			const int 		_ret;
			const char *	_msg;
			const char *	_serviceName;	
	};

	public:
		Cluster(const std::string &filename) throw(std::exception);
		Cluster(const Cluster &);
		~Cluster();
		Cluster &	operator=(const Cluster &);

		const std::string			& getFileConfig()	const;
		const std::vector<Server>	& getAllServer()	const;
		const std::set<std::string>	& getListenList()	const;

	private:
		std::set<std::string>	_listenList;		// liste de tous les ports
		std::set<std::string>	_incudeList;		// ??
		std::vector<Server>		_servers;			// ensemble des servers present dans le cluster
		std::set<int>			_sockFds;			// ensemble des fd socket serveur, destine a poll
		std::string				_configPath;		// chemin vers fichier de config
		std::string				_defaultType;		// pour default entete http
		int						_workerConnexion;	// nb total de connexion supportes par le cluster
		int						_keepAliveTime;		// le temps que le serveur garde une conneion active entre deyux requetes (secondes)
		int						_epollFd;			// fd vers structure epoll

		void	setParams();		// init les parametres (provisoir en attendant parsing)

		void	setAllSocket();
		void	setEpollFd();
		void	closeFdSet();
		void	safeSetSocket(const struct addrinfo *, int &) const throw(InitException);
		void	safeLinkSocket(const int, const struct addrinfo *, const char *) const throw(InitException);
		void	safeGetAddr(const char *, const struct addrinfo &, struct addrinfo **) const throw(InitException); 


};

std::ostream	& operator<<(std::ostream &, const Cluster &);

#endif
