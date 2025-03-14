// check si le autoindex est activer ou pas 
// si pas activer renvoyer : 403 Forbidden HTTP error-page
// je dois check si le dossier a ou pas un fichier index.
// si il n'a pas alors je dois generer une page dynamique avec l'ensemble des page possible sur le server. ?
// donc un header qui va etre semi-modulable
// et un body modulable
// ensuite juste un send au bon client et response

// utilisation de : opendir, readdir and closedir pour lister les fichiers
#include "Server.hpp"
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include "CGI.hpp"

int controlAutoIndex(Server server, std::string path)
{
    const std::set<std::string>& locations = server.getLocationPath();
    
    if (locations.find(path) != locations.end())
        return 0;
    return -1; 
}

std::string processAutoIndex(Server server, std::string path)
{
    DIR *dir;
    std::string body;
    std::string header;
    std::string response;

    if (controlAutoIndex(server, path) == -1)
        throw ErrorCGI("Index Listing Forbidden", 403); // return error 403 index listing forbidden
    if ((dir = opendir(path.c_str())) == NULL)
        throw ErrorCGI("Not Found", 404); // return 404 not found
    body = buildBodyAutoIndex(dir, path);
    header = buildHeaderAutoIndex(body);
    response += header + body;
    closedir(dir);
    return response;
}

// fct qui a pour but de creer la page a renvoyer finale
std::string buildBodyAutoIndex(DIR *dir, std::string path)
{
    struct dirent *files;
    std::string body;

  
    body = "<html>\n<head>\n<title>Telechargement Valide</title>\n</head>\n<body>\n<h1>List Files:</h1>\n";
    while ((files = readdir(dir)) != NULL)
    {
        if (std::string(files->d_name).compare(".") == 0 || std::string(files->d_name).compare("..") == 0)
            continue;
        body += "\n<a href=\"" + path + "/" + std::string(files->d_name) + "\">" + std::string(files->d_name) +  "</a>"; // ici jcrois il faut mettre d'autre balise pour link
    }
    body += "\n</body></html>";
    return body;
}

std::string buildHeaderAutoIndex(std::string body)
{
    std::string header;
    header += "Content-Type: text/html; charset=utf-8";
    header += "\nContent-Length:" + UtilParsing::intToString(body.length());
    header += "\nHTTP/1.1 200 OK\n\n"; // je mets deux \n a voir si il y a vraiment une norme car jai rien vu dessus

    return header;
}


// _____________________ PARTIE TEST ______________________

// dans cette version il manque le controle autoindex
std::string processAutoIndexTEST(std::string path)
{
    DIR *dir;
    std::string body;
    std::string header;
    std::string response;
    
    if ((dir = opendir(path.c_str())) == NULL)
        throw ErrorCGI("Not Found", 404); // return 404 not found
    body = buildBodyAutoIndex(dir, path);
    header = buildHeaderAutoIndex(body);
    response += header + body;
    closedir(dir);
    return response;
}
