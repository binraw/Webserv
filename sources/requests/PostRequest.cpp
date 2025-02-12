




#include "PostRequest.hpp"

PostRequest::PostRequest(const std::string &) : ARequest()
{

}

PostRequest::PostRequest(const PostRequest &) : ARequest()
{

}

PostRequest::~PostRequest()
{

}

PostRequest &PostRequest::operator=(const PostRequest &)
{
	return *this;
}

std::ostream & operator<<(std::ostream &o, const PostRequest&)
{
	return o;
}
