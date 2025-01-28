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

class Cluster;

class Server
{
	public:
		Server(const std::vector<std::string> & data, Cluster &);
		Server(const Server &);
		~Server();

		const int					& getBacklog() const;
		const std::set<std::string>	& getNameList() const;

	private:
		Server & operator=(const Server &);
		std::set<std::string>	_nameList;
		const int				_backLog; 	// file d'attente de connexion par socket

		void	setParams(std::vector<std::string> &);

};

/*	* OVERLOAD OUTSTREAM
	*
*/
std::ostream	& operator<<(std::ostream &, const Server &);

#endif
