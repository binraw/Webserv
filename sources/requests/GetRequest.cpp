



#include "GetRequest.hpp"

GetRequest::GetRequest(const std::string &response)
  : ARequest(response)
{

}

GetRequest::GetRequest(const GetRequest &ref)
  :	ARequest()
{
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
	}
	return *this;
}

ARequest *	GetRequest::createGetRequest(const std::string &data) {
	return new GetRequest(data);
}
