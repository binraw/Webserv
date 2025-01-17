


 



// INCLUDES UNIQUEMENT POUR LE DEVELOPPEMENT
#include <iostream>

#define PORT1   8080
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/*
    includes du fichier
    ne pas supprimer
*/
#include "webserv.hpp"
#include "Cluster.hpp"

int main (int argc, char **argv)
{
    if (argc > 2) {
        return (PRINTUSAGE, 1);
    }

    std::string configPath;

#ifdef TEST
    configPath = PATH_DEBUGSERV;
#else
    argc == 1 ? configPath = PATH_DEFAULTSERV : configPath = argv[1];
#endif

    try
    {
        Cluster cluster = Cluster(configPath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    



    /*
        * int socket(int __domain, int __type, int __protocol)
            * cree un nouveau socket de communication.
        
        * int __domain
        * AF_INET ->        domaine IPV4
        
        * int __type
        * SOCK_STREAM ->    le transfert de donnees entre l'interface reseau et l'application 
                                se fait par flux de donnee et non par datagramme ( TCP )
        * SOCK_NONBLOCK ->  le socket est non bloquant (voir ce que cela implique plus tard)
                                d'apres la doc, evite d'appeler fcntl() de maniere abusive
        
        * int __protocole
        * 
    */
    int sockFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, protoName->p_proto);
    if (sockFd < 0) {
        std::cerr   << RED ERROR
            << "function: socket() at [" << __LINE__
            << "] in file :" << __FILE__
            << RESET << std::endl;
        return 2;
    }

    /*
        * int setsockopt(int __fd, int __level, int __optname, const void *__optval, socklen_t __optlen)
            la particularrite de cette fonction c'est que les options ne peuvent pas etre chainees par un |
            il faut appeler la fonction autant de fois qu'il faut set un parametre
        * setsockopt() est une fonction tres puissante qui permet un parametrage avance sur differentes
            couches du protocole de communication. 
        * le niveau de la couche sur laquel on agit est definit dans le champ level, ainsi on peut parametrer :
            -> au niveau de la couche application, (API socket, globalement la gestion du fd par l'os).
            -> au niveau de la couche transport (parametres specifique au protocole TCP ou UDP suivant celui utilise par le socket)
            -> au niveau de la couche reseau ( parametres specifique au protocole IP )
    */
    // cet appel
    int opt = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr   << RED ERROR
            << "function: setsockopt() at [" << __LINE__
            << "] in file :" << __FILE__
            << RESET << std::endl;
        close(sockFd);
        return 2;
    }



// #if (TEST )
    // lauch server with test_file.conf
    // Cluster main(PATH_DEBUGSERV)
// #else
    // lauch server with default_server.conf
    // Cluster main(PATH_DEFAULTSERV)
// #endif

    return (0);
}