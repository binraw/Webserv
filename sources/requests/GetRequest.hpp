




#ifndef GETREQUEST_HPP
# define GETREQUEST_HPP

# include "webserv.hpp"
# include "ARequest.hpp"

class GetRequest : virtual public ARequest
{
	public:
		GetRequest(const std::string &);
		~GetRequest();

		ARequest * createGetRequest(const std::string &);

	private:
		GetRequest(const GetRequest &);
		GetRequest &operator=(const GetRequest &);


};

#endif
