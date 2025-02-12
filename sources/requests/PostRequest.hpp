




#ifndef POSTREQUEST_HPP
# define POSTREQUEST_HPP

# include "webserv.hpp"
# include "ARequest.hpp"

class PostRequest : virtual public ARequest
{
	public:
		PostRequest(const std::string &);
		PostRequest(const PostRequest &);
		~PostRequest();
		PostRequest &operator=(const PostRequest &);

	private:
		std::vector<std::string>	_arguments;
		std::string					_url;
		


};

std::ostream & operator<<(std::ostream &, const PostRequest&);

#endif
