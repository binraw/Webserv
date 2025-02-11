





#ifndef AREQUEST_HPP
# define AREQUEST_HPP

# include "webserv.hpp"

class ARequest
{
	class   RequestException : virtual public std::exception
	{
		public:
			RequestException(const std::string &msg)
				: _msg(msg)
			{	}
			virtual ~RequestException() throw() {}
			virtual const char *	what() const throw() {
				return _msg.c_str();
			};
		private:
			std::string	_msg;
	};

	public:
		ARequest(const std::string &);
		virtual ~ARequest() {};
		virtual	ARequest *	createRequest(const std::string &) = 0;

	protected:
		const std::vector<std::string>	_responseByToken;
		
		bool	_keepAlive;
		std::string	_url;
		std::string	_requestType;
		std::string	_protocolVersion;
		std::string	_formatsAccepted;
		std::pair<std::string, std::string>	_host;
	
		ARequest(const std::vector<std::string> &);
	
	private:
		// void	_setUrl();
		// void	_sethost();
		// void	_setKeepAlive();
		// void	_setRequestType();
		// void	_setProtocolVersion();
		// void	_setformatsAccepted();
		static std::string	_requestHandle[];
};

#endif
