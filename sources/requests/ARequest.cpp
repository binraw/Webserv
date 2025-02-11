

/*============================================================================*/
								/*### HEADERS ###*/
/*============================================================================*/


#include "ARequest.hpp"
#include "UtilParsing.hpp"

/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/
ARequest::ARequest(const std::string &response)
{
	_respponseByToken = UtilParsing::split(response, " \r\n");
	
}

ARequest::ARequest(const ARequest &)
{   }


/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### EXCEPTIONS ###*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/
