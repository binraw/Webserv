


#ifndef REQUESTBUILDER_HPP
# define REQUESTBUILDER_HPP

# include "webserv.hpp"
# include "GetRequest.hpp"
# include "PostRequest.hpp"

typedef	ARequest* (*ptrToRequestBuilder)(const std::string&);

class RequestBuiler
{
	public:
		virtual ~RequestBuiler() {};

		static ARequest *createRequest(const std::string &) {
			
		};
	
	private:
		ARequest *buildGetRequest(const std::string &unparsedRequest) {
			return new GetRequest(unparsedRequest);
		}
		ARequest *buildPostRequest(const std::string &unparsedRequest) {
			return new PostRequest(unparsedRequest);
		};
		static const std::map<std::string, ptrToRequestBuilder> requestTable;

};

#endif
