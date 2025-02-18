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


    std::string _pathCGI;
    
public:

    ARequest* request;
    std::string _response;
    std::string _url;
    std::string _filename;
    std::string _contentBody;
    std::string _codeResponse;
    std::string _contentType;
    std::string _contentLength;

    Client(std::string file);
    int executeGetRequest();
    int writeGetResponse();
    int executeDeleteRequest();
    int writeDeleteResponse();
    int checkPossibilityFile();
    int checkRequest();
    void buildCodeResponse(int code);
    void buildResponse();
    ~Client();
    Client &operator=(const Client &);
};





#endif
