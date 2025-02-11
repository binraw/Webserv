





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
		std::vector<std::string>	_fileByToken;

		bool		_connexionType; //true == keepalive
		std::string	_host;	//identifie le domaine recherche par le client format servername:port
		std::string	_url;	//chemin vers la requete du client
		std::string	_userAgent;
		std::string	_requestType;
		std::vector<std::string>	_respponseByToken;


};
std::ostream	& operator<<(std::ostream &, const ARequest &);

#endif
