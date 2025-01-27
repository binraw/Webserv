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


typedef struct s_paramServer
{
    std::map<std::string, std::set<std::string> > params;
    s_paramServer()
    {
        params["server_name"].insert("localhost");
        params["listen"].insert("8080");
        params["listen"].insert("8081");
        params["listen"].insert("8081");
        params["client_max_body_size"].insert("200M");
        params["upload_path"].insert("./upload");
    }
}   t_paramServer;

class Server
{
	public:
	/*	* CANONICAL FORM
		*
	*/
		Server(const std::vector<std::string> & data) throw(std::exception);
		Server(const Server &);
		~Server();

	/*	* PUBLIC METHODS
		*
	*/
		std::vector<std::string>	addValuesRoads(std::vector<std::string>::iterator &cursor); // creer vector avec les 'localisation' differentes roads du server
		void						initDefaultConfServ(); // ou tout ce lance + seraparation default et diffente road
		void						createMapDefaultConf(); // map avec default conf des serveurs
	
	/*	* GETTERS
		*
	*/
		const t_paramServer	& getParams() const;
		const std::set<int>	& getFdSet() const;
		const int			& getBacklog() const;
		
	private:
	/*	* PRIVATE OPERATOR
		*
	*/
		Server & operator=(const Server &);

	/*	* PRIVATE MEMBERS
		*
	*/
		t_paramServer	_params;	// server default params
		std::set<int>	_fdSet;		// ensemble des sockets en ecoute sur le serveur (un par port)
		const int		_backLog; 	// file d'attente de connexion par socket

		std::vector<std::string>					_basicData;			// donnee donner par le cluster sur toutes les infos du server
		std::vector<std::string>					_defaultConfServer;	// vector avec infos par default du serveur
		std::map<int, std::vector<std::string> >	_vectRoads;			// map avec les different localisation du server
		std::map<std::string, std::string>			_mapConfDefault;	// map ranger des valeurs default key + value

	/*	* SETTERS
		* 
	*/
		void	setParams(std::vector<std::string> & token);
		void	setSocket()	throw(InitException);

	/*	* PRIVATE METHODS
		*
	*/
		void	setSockOptSafe(const struct addrinfo *currNode, int &fd)				const throw(InitException);
		void	linkSocket(const int, const struct addrinfo *, const char *currPort)	const throw(InitException);
		void	closeFdSet();

	/*	* EXCEPTIONS
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

/*	* OVERLOAD OUTSTREAM
	*
*/
std::ostream	& operator<<(std::ostream &, const Server &);
std::ostream	& operator<<(std::ostream &, const t_paramServer &);

#endif
