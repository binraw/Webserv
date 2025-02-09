




#ifndef GETREQUEST_HPP
# define GETREQUEST_HPP

# include "webserv.hpp"
# include "ARequest.hpp"

class GetRequest : virtual public ARequest
{
	public:
		GetRequest(const std::string &);
		GetRequest(const GetRequest &);
		~GetRequest();
		GetRequest &operator=(const GetRequest &);

		GetRequest *duplicate();

	private:

};
std::ostream	& operator<<(std::ostream &, const GetRequest &);

#endif
