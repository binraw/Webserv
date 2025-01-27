#ifndef SERVER_HPP
# define SERVER_HPP

# include "UtilParsing.hpp"
# include "../includes/webserv.hpp"

# include <stdexcept>
# include <iostream>
# include <cstring>
# include <cerrno>
# include <string>

# include <vector>
# include <set>
# include <map>

# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <netdb.h>


class InitException;

typedef struct s_paramsServer
{
	std::map<std::string, std::set<std::string> > params;
	s_paramsServer() 
	{
		params["server_name"].insert("localhost");
		params["listen"].insert("8000");
		// params["listen"].insert("http");
		// params["listen"].insert("8001");
		// params["listen"].insert("8002");
		// params["listen"].insert("8003");
		params["client_max_body_size"].insert("200M");
		params["upload_path"].insert("./upload");
		
	}
} t_paramServer;

class Server
{
	public:
		Server(const std::vector<std::string> & data) throw(std::exception);
		Server(const Server &);
		~Server();
		Server & operator=(const Server &);

	/*	* Parsing public methods
		*
	*/
		std::vector<std::string>	addValuesRoads(std::vector<std::string>::iterator &cursor); // creer vector avec les 'localisation' differentes roads du server
		void						initDefaultConfServ(); // ou tout ce lance + seraparation default et diffente road
		void						createMapDefaultConf(); // map avec default conf des serveurs
		
	/*	* Getters
		* 
	*/
		t_paramServer	& getParams()	const;
		std::set<int>	& getFdSet()	const;
		int				getBacklog()	const;
		
	private:
	/*	* Setters
		* 
	*/
		void			setParams(std::vector<std::string> & token);
		void			setSocket()	throw(InitException);
		
	/*	* Private members
		* 
	*/
		t_paramServer	_params;	// parametres par default du serveur
		std::set<int>	_fdSet;		// l'ensemble des fd du serveur (un pour chacun de ses ports d'ecoute)
		const int		_backLog;	// = Cluster::_defaultParams.params[worker_connexion]
		std::vector<std::string>					_basicData;			// donnee donner par le cluster sur toutes les infos du server
		std::vector<std::string>					_defaultConfServer;	// vector avec infos par default du serveur
		std::map<int, std::vector<std::string> >	_vectRoads;			// map avec les different localisation du server
		std::map<std::string, std::string>			_mapConfDefault;	// map ranger des valeurs default key + value

	/*	* socket setters methods
		* 
	*/
		void	setSockOptSafe(const struct addrinfo *currNode, int &fd) 				const throw(InitException);
		void	linkSocket(const int, const struct addrinfo *, const char *currPort)	const throw(InitException);
		void	closeFdSet() const;

	/*	* Class exception
		* 
	*/
		class   InitException : virtual public std::exception
		{
			public:
				InitException(const char *file, int line, const char *msg, const char *port)
				  : _file(file), _line(line), _msg(msg), _port(port)
				{	}
				const char *	what() const throw();
				void			setSockExcept() const throw();

			private:
				const char *	_file;
				const int 		_line;
				const char *	_msg;
				const char *	_port;			
		};
};

std::ostream	& operator<<(std::ostream &, const Server &);
std::ostream	& operator<<(std::ostream &, const t_paramServer &);

#endif
