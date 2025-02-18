
#include "Client.hpp"

// Client::Client(int fd, std::string request)
// {
// }

// ok ne pas oublier de set script.pl comme fichier par default si l'utilisateru envoie juste cgi-bin/ comme chemin 

Client::Client(std::string _response)
{
    // if (request == "cgi-bin/")
    //     _pathCGI = "cgi-bin/script.pl";
    // else
    // {
    //     _pathCGI = request;
    // }
        
}
Client::~Client()
{
}

Client &Client::operator=(const Client &)
{
    return *this;
}

// -------------LIRE AVANT TOUT CHANGEMENT -----------------------------
// l'idee ca va etre que la on a le body de chaque Method
// maintenant il va falloir rajouter le header a chaque response 
// le body sera enregistrer dans _contentBody avant d'etre transferer a la valeur response
// ne pas prendre en compte les noms des variables utiliser dans le code ci-dessous 
// il va etre changer au moment ou nos deux codes seront liees
// ----------------------------------------------------------------------
int Client::checkRequest() 
{
    if (_response.find("POST") != std::string::npos)
        request = new PostRequest(_response); // Post va etre transferer dans cette class au prochain push
    else if (_response.find("GET") != std::string::npos)
    {
        executeGetRequest();
    }
    else if (_response.find("DELETE") != std::string::npos)
    {
        executeDeleteRequest();
    }
    else
    {
        return 400; // Bad Request
    }
}

int Client::executeGetRequest()
{
    if (_url.find("GET") != std::string::npos) // faut il faire aussi un fork pour renvoyer le bon file ?
    {
        if (UtilParsing::fileExits(_filename))
        {
            return writeGetResponse(); // la je rempli le futur body mais il faut remplir les en-tete
        }
        else
            return 400;
    }
    return 404;
}

int Client::writeGetResponse()
{
    std::ifstream file(_filename.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();
    _contentBody = buffer.str();
    if (_contentBody.empty())
        return 404;
    return 200;
}

int Client::executeDeleteRequest()
{
    if (_url.find("DELETE") != std::string::npos)
    {
        if (UtilParsing::fileExits(_filename))
        {
            if (checkPossibilityFile() == 0)
               if (writeDeleteResponse() == 0)
                    return 200;
        }
        else
            return 400;
    }
    return 404;
}

int Client::writeDeleteResponse()
{
    if (remove(_filename.c_str()) == 0)
    {
        _contentBody = "<body><h1>Suppression de " + _filename + " effectué</h1></body>";
        return 0;
    }
    else
        return 1;
}

int Client::checkPossibilityFile()
{
    if (_filename.find("upload/") != std::string::npos)
    {
        if (_filename.find("..") == std::string::npos) // ici on verifie qu'il n'y a pas de ..
        {
            return 0;
        }
    }
    return 1;
}


// La derniere fonction pour la renvoyer la response complete pour le server
// elle ne sera fonctionnel seulement apres la liaison des codes 
void Client::buildResponse()
{
    _response = "HTTP/1.1"  + _codeResponse + "\nContent-Type: " + _contentType + "\nContent-Length: " + _contentLength 
                + _contentBody;
}

void Client::buildCodeResponse(int code)
{
    if (code == 400)
        _codeResponse = "400 Bad Request";
    else if (code == 401)
        _codeResponse = "401 Unauthorized";
    else if (code == 403)
        _codeResponse = "403 Forbidden";
    else if (code == 404)
        _codeResponse = "404 Not Found";
    else if (code == 401)
        _codeResponse = "401 Unauthorized";
    else if (code == 405)
        _codeResponse = "405 Method Not Allowed";
    else if (code == 408)
        _codeResponse = "408 Request Timeout";
    else if (code == 500)
        _codeResponse = "500 Internal Server Error";
    else if (code == 501)
        _codeResponse = "501 Not Implemented";
    else if (code == 502)
        _codeResponse = "502 bad Gateway";
    else if (code == 503)
        _codeResponse = "503 Service Unavailable";
    else if (code == 200)
        _codeResponse = "200 OK";
    else if (code == 201)
        _codeResponse = "201 Created";
    else if (code == 202)
        _codeResponse = "202 Accepted";
    else
        _codeResponse = "204 No Content";
}