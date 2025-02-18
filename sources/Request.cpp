


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
						/*### HEADERS & STATIC FIELD ###*/
/*============================================================================*/

#include "Request.hpp"
#include "UtilParsing.hpp"

/*============================================================================*/
				/*### CONSTRUCTORS - DESTRUCTOR - OVERLOAD OP ###*/
/*============================================================================*/

/*	* if an error is detected throw an exception with the correct error code
	* http version
	* method type
	* path to ressource
	* Host error
*/
Request::Request(const std::string &response)
{
	size_t			idxSeparator = response.find(BODY_SEPARATOR);
	std::string		header = response.substr(0, idxSeparator + 1);
	static size_t	separatorSize = std::string(BODY_SEPARATOR).length();	
	
#ifdef TEST
	std::cout << response << std::endl;
#endif
	_body = response.substr(idxSeparator + separatorSize, response.length() - idxSeparator);

	const std::vector<std::string>	tokenHeader = UtilParsing::split(header, "\r\n");
	std::vector<std::string>::const_iterator	itToken = tokenHeader.begin();
	
	initRequestLine(*itToken);
	initHost(++itToken, tokenHeader.end());
	_keepAlive = response.find("keep-alive") == response.npos ? false : true;
	std::cout << *this << std::endl;
}
/*----------------------------------------------------------------------------*/

Request::Request(const Request &ref) {
	*this = ref;
}
/*----------------------------------------------------------------------------*/

Request::~Request()
{	}
/*----------------------------------------------------------------------------*/

Request & Request::operator=(const Request &ref)
{
	if (this != &ref)
	{
		_keepAlive = ref._keepAlive;
		_uri = ref._uri;
		_hostName = ref._hostName;
		_hostPort = ref._hostPort;
		_requestType = ref._requestType;
		_body = ref._body;
	}
	return *this;
}
/*----------------------------------------------------------------------------*/

std::ostream & operator<<(std::ostream & o, const Request &ref)
{
	o	<< BOLD BRIGHT_BLUE "Request:" << std::endl
		<< "_requestType: [" << ref.gettype() << "]" << std::endl
		<< "_uri: [" << ref.geturi() << "]" << std::endl
		<< "_hostName: [" << ref.gethostname() << "]" << std::endl
		<< "_hostPort: [" << ref.gethostport() << "]" << std::endl
		<< "_keepAlive: [" << (ref.getkeepalive() == true ? "true" : "false") << "]" << std::endl
		<< std::endl
		<< "BODY :\n" << ref.getbody() << RESET;
	return o;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PUBLIC METHODS ###*/
/*============================================================================*/

bool Request::getkeepalive() const {
	return _keepAlive;
}
/*----------------------------------------------------------------------------*/

const std::string	Request::geturi() const {
	return _uri;
}
/*----------------------------------------------------------------------------*/

const std::string	Request::gethostname() const {
	return _hostName;
}
/*----------------------------------------------------------------------------*/

const std::string	Request::gethostport() const {
	return _hostPort;
}
/*----------------------------------------------------------------------------*/

const std::string	Request::gettype() const {
	return _requestType;
}
/*----------------------------------------------------------------------------*/

const std::string	Request::getbody() const {
	return _body;
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### PRIVATE METHODS ###*/
/*============================================================================*/

/*	* check validity of the request line (METHOD PATH_TO_RESSOURCE PROTOCOL_VERSION)
	* throw exception with the correct error code
*/
void	Request::initRequestLine(const std::string &requestLine)
{
	_requestType = requestLine.substr(0, requestLine.find_first_of(" "));
	//throw exception with error 415 -> Unsupported Media Type : Format de requête non supporté pour une méthode et une ressource données.
	if (_requestType.empty() == true) {
		std::cerr << RED "throw exception with error 415 in initRequestLine()" RESET << std::endl;
		throw std::exception();
	}

	//throw exception with error 400 -> Bad request : protocol non supporte
	if (requestLine.find(PROTOCOL_VERION) == std::string::npos) {
		std::cerr << RED "throw exception with error 400 in initRequestLine() : " YELLOW << requestLine << RESET << std::endl;
		throw std::exception();
	}

	//error maybe impossible
	size_t	idx = requestLine.find_first_of("/");
	if (idx == std::string::npos) {
		std::cerr << RED "\'/\' DOESN'T FIND IN requestLine in function initRequestLine()" RESET << std::endl;
		throw std::exception();
	}
	_uri = requestLine.substr(idx, requestLine.find_first_of(' ', idx) - idx);
}

/*	* extract the hostname and the host port required by the client
*/
void	Request::initHost(std::vector<std::string>::const_iterator &itToken, std::vector<std::string>::const_iterator itEnd)
{
	while (itToken != itEnd) {
		if (itToken->find("Host") != itToken->npos)
			break;
		itToken++;
	}
	if (itToken == itEnd) {
		std::cerr << RED "NO HOST in initHost()" << std::endl; // manage error
		return;
	}
	
	_hostName.clear();
	_hostPort.clear();
	size_t idxSpace = itToken->find_last_of(" ");
	size_t idxSemicolon = itToken->find_last_of(":");
	_hostName = itToken->substr(idxSpace + 1, idxSemicolon - idxSpace - 1);
	_hostPort = itToken->substr(idxSemicolon + 1, itToken->length() - idxSemicolon);
}
/*----------------------------------------------------------------------------*/

/*============================================================================*/
						/*### EXCEPTIONS ###*/
/*============================================================================*/


/*----------------------------------------------------------------------------*/
