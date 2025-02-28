
// check l'existance du file a l'endroit donner (on a deja la fct)
// check si le script demander est executable avant tout (avec access)
// verifier aussi l'extension appeler est celle gerer par le server
// se deplacer dans le bon repertoire avant d'exec le script

// construire le header ? ou il sera construit dans les cgi ? 
// je pense qu'il faut en formatter un par default
// une possibilite de creer un fichier env ? pas sur de cette info ca serait sans doute 
//un truc en plus a pouvoir 
// set mais bon dans le sujet pas tres utile

// la fragmentation sera fais avant ou je dois l'implementer dans le processus question a explorer

// infos trouvees:
// Tous les segments de chemin restants sont transmis au script dans la variable d'environnement PATH_INFO.

// Le mot-clé chunked dans l' en-tête Transfer-Encoding est utilisé pour indiquer un transfert fragmenté.
//La transmission se termine lorsqu'un fragment de longueur nulle est reçu.
// la fonction: fcntl peut etre utile si il faut bloquer l'acces d'un fichier pendant un processus
// mais actuellement je sais pas si c'est vraiment utile


// apres des recherche je trouve qu'il faut justement defenir notre propre environnement avec les info envoyer 
// du processus parent pour le processus enfant.
// les diffenrente values presente pour l'env serait : 
// REQUEST_METHOD: Spécifie la méthode HTTP utilisée pour accéder au script
// QUERY_STRING: Contient les données de la requête HTTP sous forme de chaîne de caractères 
// (par exemple, param1=value1&param2=value2)
// CONTENT_TYPE: Spécifie le type de contenu de la requête HTTP
// CONTENT_LENGTH: Spécifie la longueur des données de la requête HTTP
// HTTP_HOST: Spécifie le nom d'hôte du serveur Web
// HTTP_REFERER: Spécifie l'URL de la page Web qui a envoyé la requête
// REMOTE_ADDR: Spécifie l'adresse IP du client qui a envoyé la requête
// REMOTE_PORT: Spécifie le port utilisé par le client pour envoyer la requête
// SCRIPT_NAME: Spécifie le nom du script CGI
// PATH_INFO: Spécifie les informations de chemin supplémentaires qui ont été passées au script
// (par exemple, /path/to/resource)
#include "Client.hpp"

void checkFileScript(const std::string &path, Server server)
{
    if (UtilParsing::fileExits(path) == 0)
    {
        if (access(path.c_str(), X_OK) == 0)
        {
            if (checkExtensionCGI(path, server) == 0)
            {
                
                if (moveToDirectoryScript(extractDirectory(path)) == 0)
                {

                }

            }
        }
    }

}

bool checkExtensionCGI(const std::string &path, Server server)
{
   std::string upload_path = server.getConfig()._uploadPath;

    if (UtilParsing::recoverExtension(path) == UtilParsing::recoverExtension(upload_path))
        return true;
    return false;
}

std::string extractDirectory(const std::string &path)
{
    std::string directory;

    size_t posEndDirectory = path.find_last_of("/");
    directory = path.substr(0, posEndDirectory);

    return directory;
}


bool moveToDirectoryScript(const std::string &directory)
{
    if (chdir(directory.c_str()) != 0)
    {
        std::cerr << "Error move directory script." << std::endl;
        return false;
    }
    return true;
}

char *initEnv()
{
//     // ici initialiser l'env avec les bonne values
 
}


// 