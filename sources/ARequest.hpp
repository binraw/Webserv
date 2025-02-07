





#ifndef AREQUEST_HPP
# define AREQUEST_HPP

# include "../includes/webserv.hpp"

class ARequest
{
	public:
		ARequest(const std::string &);
		ARequest(const ARequest &);
		~ARequest();

	private:
		ARequest	*request;

		ARequest & operator=(const ARequest &);
		void	setParams(std::vector<std::string> &);


};
std::ostream	& operator<<(std::ostream &, const ARequest &);

#endif
