





#ifndef AREQUEST_HPP
# define AREQUEST_HPP

# include "webserv.hpp"

class ARequest
{
	public:
		ARequest(const std::string &);
		ARequest(const ARequest &);
		virtual ~ARequest() {};

		ARequest *newRequest(const std::string &);
	protected:
		bool	_connexionType; //true == keepalive
		std::string	_host;	//identifie le domaine recherche par le client format servername:port
		std::string	_url;	//chemin vers la requete du client
		std::string	_userAgent;
		std::string	_requestType;
		std::vector<std::string>	_filesTypesAccepted;
		

		ARequest & operator=(const ARequest &);
		void	setParams(std::vector<std::string> &);


};
std::ostream	& operator<<(std::ostream &, const ARequest &);

#endif
