





#ifndef AREQUEST_HPP
# define AREQUEST_HPP

# include "webserv.hpp"

class ARequest
{
	// class   RequestException : virtual public std::exception
	// {
	// 	public:
	// 		RequestException(const std::string &msg, int errcode)
	// 			: _msg(msg), _errcode(errcode)
	// 		{	}
	// 		virtual ~RequestException() throw() {}
	// 		virtual const char *	what() const throw() {
	// 			return _msg.c_str();
	// 		};
	// 	private:
	// 		int			_errcode;
	// 		std::string	_msg;
	// };

	public:
		ARequest(const std::string &);
		~ARequest() {};
		ARequest *	createRequest(const std::string &);

	protected:

		bool	_keepAlive;
		std::string	_url;
		std::string	_requestType;
		
		std::string	_formatsAccepted;
		
		std::string	_hostName;
		std::string	_hostPosrt;
	
		ARequest(/* const std::vector<std::string> & */);
		void	_setKeepAlive(bool);
	
	private:
		void	initRequestLine(const std::string &);
		void	initHost(std::vector<std::string>::const_iterator &, std::vector<std::string>::const_iterator);

		void	initformatsAccepted();
		static std::string	_requestHandle[];
};

#endif
