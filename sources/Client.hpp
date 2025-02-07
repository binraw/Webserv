#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../includes/webserv.hpp"
#include "ARequest.hpp"

class Client
{
	public:
		Client(const ARequest &);
		Client(const Client &);
		~Client();

	private:
		ARequest	*request;

		Client & operator=(const Client &);
		void	setParams(std::vector<std::string> &);


};
std::ostream	& operator<<(std::ostream &, const Client &);

#endif
