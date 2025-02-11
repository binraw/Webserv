

/*============================================================================*/
						/*### HEADERS & STATIC FIELD###*/
/*============================================================================*/

#include "ARequest.hpp"
#include "UtilParsing.hpp"

static std::string requestHandle[] =
{
	"GET",
	"POST",
	"DELETE"
};

/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/
ARequest::ARequest(const std::string &response)
  :	_responseByToken(UtilParsing::split(response, " \r\n")), _keepAlive(true)
{
	// _setformatsAccepted();
	// _sethost();
	// _setKeepAlive();
	// _setProtocolVersion();
	// _setRequestType();
	// _setUrl();
}

ARequest::ARequest(const std::vector<std::string> &tokenResponse)
  :	_responseByToken(tokenResponse), _keepAlive(true)
{	}


/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/
// void	ARequest::_setUrl()
// {

// }
// /*----------------------------------------------------------------------------*/

// void	ARequest::_sethost()
// {

// }
// /*----------------------------------------------------------------------------*/

// void	ARequest::_setKeepAlive()
// {

// }
// /*----------------------------------------------------------------------------*/

// void	ARequest::_setRequestType()
// {
// 	for (std::string::iterator it = _requestHandle->begin(); it != _requestHandle->begin(); it++)
// 	{

// 	}


// }
// /*----------------------------------------------------------------------------*/

// void	ARequest::_setProtocolVersion()
// {

// }
// /*----------------------------------------------------------------------------*/

// void	ARequest::_setformatsAccepted()
// {

// }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### EXCEPTIONS ###*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/
