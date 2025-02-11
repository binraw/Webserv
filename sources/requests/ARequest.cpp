

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

static std::string requestHandle[] =
{
	"GET",
	"POST",
	"DELETE"
};

/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/

/*	* if an error is detected throw an exception with the correct error code
	* http version
	* method type
	* path to ressource
*/
ARequest::ARequest(const std::string &response)
{
	const std::vector<std::string>	token = UtilParsing::split(response, "\r\n");
	std::vector<std::string>::const_iterator itToken = token.begin();
	UtilParsing::displayVector(UtilParsing::split(response, "\n\r"));

	initRequestLine(*itToken);

	// while (itToken != token.end())
	// {
	// 	if (itToken->find() )
	// }
	// initMandatoryFields();

	// initformatsAccepted();
}
/*----------------------------------------------------------------------------*/

ARequest::ARequest(/* const std::vector<std::string> &tokenResponse */)
{	}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

void	ARequest::initRequestLine(const std::string &requestLine)
{
	std::cout << "requestLine: " << requestLine << std::endl;
	// this->_protocolVersion;
	// this->_requestType;
	// this->_url;


}

void	ARequest::initMandatoryFields()
{
	// try {
	// 	_responseByToken.
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	
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
