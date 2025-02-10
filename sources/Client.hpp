#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "ARequest.hpp"

class Client
{
	public:
		Client(const int);
		Client(const Client &);
		~Client();
		Client	&operator=(const Client &);

		const ARequest *getRequest() const;

	private:
		ARequest	*_request;
		std::string	_response;
};

std::ostream & operator<<(std::ostream &, const Client &);

#endif
