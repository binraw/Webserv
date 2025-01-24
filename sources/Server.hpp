#ifndef SERVER_HPP
# define SERVER_HPP

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

#include "UtilParsing.hpp"
#include "../includes/webserv.hpp"

class InitException;

typedef struct s_paramsServer
{
	std::map<std::string, std::set<std::string> > params;
	s_paramsServer() 
	{
		params["server_name"].insert("localhost");
		params["listen"].insert("8000");
		params["listen"].insert("443");
		params["listen"].insert("8001");
		params["client_max_body_size"].insert("200M");
		params["upload_path"].insert("./upload");
		
	}
} t_paramServer;

class Server
{
	public:
		Server();
		Server(const std::vector<std::string> &);
		Server(const Server &);
		~Server();
		Server & operator=(const Server &);

		std::vector<std::string>	addValuesRoads(std::vector<std::string>::iterator &cursor); // creer vector avec les 'localisation' differentes roads du server
		void						initDefaultConfServ(); // ou tout ce lance + seraparation default et diffente road
		void						createMapDefaultConf(); // map avec default conf des serveurs

		t_paramServer				& getParams() const;
		std::set<int>				& getFdSet() const;
		void	closeFdSet() const ; // provisoirement en public pour les tests
		
	private:
		/*
			parsing setters members & methods
		*/
		void			setParams(std::vector<std::string> & token);
		t_paramServer	_params;

		std::vector<std::string>					_basicData; // donnee donner par le cluster sur toutes les infos du server
		std::vector<std::string>					_defaultConfServer; // vector avec infos par default du serveur
		std::map<int, std::vector<std::string> >	_vectRoads; // map avec les different localisation du server
		std::map<std::string, std::string>			_mapConfDefault; // map ranger des valeurs default key + value

		/*
			socket setters members & methods
		*/
		void	setSocket();
		void	freeAllServer(const std::string &file, const int line, const std::string & msg) const;
		void	setSockOptSafe(int fd, struct addrinfo * res);

		std::set<int>	_fdSet;
		const int		_backLog; // = Cluster::_defaultParams.params[worker_connexion]

		class   InitException : virtual public std::exception
		{
			protected:
				const std::string &	_file;
				const int 			_line;
				const std::string &	_msg;
			public:
				InitException(const std::string & file, int line, const std::string & msg)
				  : _file(file), _line(line), _msg(msg)
				{	}				
				void		setOptionExcept(const int fd, const struct addrinfo * res) const throw();
				const char * what() const throw();
		};
};

std::ostream	& operator<<(std::ostream &, const Server &);
std::ostream	& operator<<(std::ostream &, const t_paramServer &);

#endif
