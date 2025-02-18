

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "Request.hpp"

class Client
{
	public:
		Client(const Request &req) {
			_request = req;
		}
		Client(){};
		~Client(){};

	private:
		Request _request;

};

// std::ostream & operator<<(std::ostream &, const Client &);

#endif

