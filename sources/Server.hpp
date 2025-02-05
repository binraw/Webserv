#ifndef SERVER_HPP
# define SERVER_HPP

# include "../includes/webserv.hpp"


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
std::ostream	& operator<<(std::ostream &, const Server &);

#endif
