

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"
# include "Request.hpp"
class Server;

class Client
{
	public:
		Client(const Request &req) {
			_request = req;
			_clientServer = NULL;
		}
		Client(){};
		~Client(){};

		Request &getrequest(){
			return _request;
		};
		Server	*_clientServer;

	private:
		Request	_request;

};

std::ostream & operator<<(std::ostream &o, Client &ref);

#endif

