



#include "GetRequest.hpp"

GetRequest::GetRequest(const std::string &response)
  : ARequest(response)
{

}

GetRequest::GetRequest(const GetRequest &ref)
  : ARequest(ref._responseByToken) {
	*this = ref;
}

GetRequest::~GetRequest()
{	}

GetRequest &	GetRequest::operator=(const GetRequest &ref)
{
	if (this != &ref)
	{
		_keepAlive = ref._keepAlive;
		_url = ref._url;
		_requestType = ref._requestType;
		_protocolVersion = ref._protocolVersion;
		_formatsAccepted = ref._formatsAccepted;
		_host = ref._host;
	}
	return *this;
}

// ARequest *	GetRequest::createRequest(const std::string &)
// {


// }



