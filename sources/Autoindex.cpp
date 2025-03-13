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


int controlAutoIndex(Server server, std::string path)
{
    const std::set<std::string>& locations = server.getLocationPath();
    
    if (locations.find(path) != locations.end())
        return 0;
    return -1; 
}

std::string BuildPageAutoIndex()
{
    
}