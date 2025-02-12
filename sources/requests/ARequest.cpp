

/*	* les requetes
	*
	* ENTETES OBLIGATOIRES
	* ligne de requete  [GET / HTTP/1.1] -> toujours en premiere ligne
	* Host:		[Host: localhost:8080]
	*
	* ENTETES CONSEILLES
	* Connection:	-> keep-alive ou non
	* Accept:		-> type mime
	*
	* ENTETES IGNORES
	* Accept-language:	-> langue favorite client
	* Accept-encoding:	-> compressions supportes par client
	* Upgrade-Insecure-Requests:	-> demande redirection https
	* 
	* AUTRES ENTETES
	* User-Agent:		-> info sur client (OS-navigateur-arch microprocesseur - serveur graphique) util pour cookies (a mon avis)
	* Cache-control:	-> pour gerer cache cote client
	* sec-ch-ua, sec-ch-ua-mobile, sec-ch-ua-platform -> infos environement client

*/

/*============================================================================*/
						/*### HEADERS & STATIC FIELD###*/
/*============================================================================*/

#include "ARequest.hpp"
#include "UtilParsing.hpp"


std::string ARequest::_requestHandle[] =
{
	"GET",
	"POST",
	"DELETE"
};



#define PROTOCOL_VERION	"HTTP/1.1"


/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/

/*	* if an error is detected throw an exception with the correct error code
	* http version
	* method type
	* path to ressource
*/
ARequest::ARequest(const std::string &response)
  :	_keepAlive(response.find("keepalive") != response.npos ? true : false)
{
	const std::vector<std::string>	token = UtilParsing::split(response, "\r\n");
	std::vector<std::string>::const_iterator itToken = token.begin();

#ifdef TEST
	std::cout	<< "Request by token: (IN ARequest constructor)" << std::endl;
	UtilParsing::displayVector(UtilParsing::split(response, "\n\r"));
#endif
	
	initRequestLine(*itToken);
	initHost(itToken, token.end());


}
/*----------------------------------------------------------------------------*/

ARequest::ARequest(/* const std::vector<std::string> &tokenResponse */)
{	}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

/*	* check validity of the request line (METHOD PATH_TO_RESSOURCE PROTOCOL_VERSION)
	* throw exception with the correct error code
*/
void	ARequest::initRequestLine(const std::string &requestLine)
{
	_requestType.clear();
	for (size_t i = 0; i < _requestHandle->size(); i++)
	{
		if (requestLine.find(_requestHandle[i]) != std::string::npos)
			_requestType = _requestHandle[i];
	}
	//throw exception with error 415 -> Unsupported Media Type : Format de requête non supporté pour une méthode et une ressource données.
	if (_requestType.empty() == true) {
		std::cerr << RED "throw exception with error 415 in initRequestLine()" RESET << std::endl;
		return;
	}

	//throw exception with error 400 -> Bad request : protocol non supporte
	if (requestLine.find(PROTOCOL_VERION) == std::string::npos){
		std::cerr << RED "throw exception with error 400 in initRequestLine()" RESET << std::endl;
		return;
	}

	//error maybe impossible
	size_t	idx = requestLine.find_first_of("/");
	if (idx == std::string::npos) {
		std::cerr << RED "\'/\' DOESN'T FIND IN requestLine in function initRequestLine()" RESET << std::endl;
		return;
	}
	_url = requestLine.substr(idx, requestLine.find_first_of(' ', idx) - idx);

#ifdef TEST
	std::cout	<< BOLD BRIGHT_BLUE "Function -> initRequestLine():" RESET << std::endl
				<< BRIGHT_BLUE "requestLine: " << requestLine << std::endl
				<< "_requestTyppe : " + _requestType << std::endl
				<< "_url: " + _url
				<< RESET << std::endl;
#endif

}

/*	* 
*/
void	ARequest::initHost(std::vector<std::string>::const_iterator &itToken, std::vector<std::string>::const_iterator itEnd)
{
	std::cout << "hostline in initHost(): " << *itToken << std::endl;

	_hostName.clear();
	_hostPosrt.clear();

	while (itToken != itEnd) {
		if (itToken->find("Host") != itToken->npos)
			break;
		itToken++;
	}
	if (itToken == itEnd) {
		std::cerr << RED "NO HOST in initHost()" << std::endl;
		return;
	}

	std::cout << "hostline in initHost(): " << *itToken << std::endl;

	
}
/*----------------------------------------------------------------------------*/

void	ARequest::initformatsAccepted()
{

}
/*----------------------------------------------------------------------------*/

void	ARequest::_setKeepAlive(bool isActiv) {
	_keepAlive = isActiv;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### EXCEPTIONS ###*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/
