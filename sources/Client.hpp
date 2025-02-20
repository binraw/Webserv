#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "ARequest.hpp"
#include "Server.hpp"
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "parsing/UtilParsing.hpp"
#include <sstream>

class Client /*: virtual public Server*/
{
private:
    // les valeurs suivantes sont la pour utiliser la fct Upload

    std::string _nameFileTeleverser;
    std::string _contentFileTeleverser;


    
public:

    std::string _body; // Contenu envoyer du client
    std::string _uri; // PATH DEMANDER
    std::string _requestType; // POST GET DELETE
    ServerConfig _config; // config du server en cours
    
    std::string _response; // renvoi final au client
    
    std::string _contentBody; // body renvoyer au client
    std::string _codeResponse; // code de la response
    std::string _contentType; // champ obligatoire a renvoyer
    std::string _contentLength; // taille du body renvoyer au client
    std::map<std::string, std::string> _mimeMap;

    Client(std::string file);
    std::string processResponse();
    int executeGetRequest();
    int writeGetResponse();
    int executeDeleteRequest();
    int writeDeleteResponse();
    int checkPossibilityFile();
    int checkRequest();
    void buildCodeResponse(int code);
    void buildResponse();
    std::string buildDeleteResponse();
    void controlCodeResponse(int code);
    std::string buildErrorPage(int code);
    void Client::buildContentType();
    std::map<std::string, std::string> initMapMime();


    // POST
    int executePostRequest();
    std::string playCGI();
    int save_file(const std::string& request_body);
    std::string extract_filename(const std::string& request_body);


    ~Client();
    Client &operator=(const Client &);
};





#endif
