#include "Server.hpp"


Server::Server(const std::string& filename)
{    }

Server::~Server()
{
}

/*
    * protoent *getprotobyname(const char *__name)

        * revoie une structure protoent qui contient:
        * char  *name -> chaine de caractere avec le nom du protocole demande (ici il sera egale a "tcp")
        * char  **alias -> tableau de chaine de caractere qui contient tousles alias du protocole (exemple alias[0] = "TCP")
        * int   num_proto -> le numero de protocole qui sera donne en dernier parametre de la fonction socket()
            afin de l'initialiser proprement et rendre le code fonctionnel sur plusieurs plateformes (WINDOWS LINUX ETC...)
*/
protoent    * Server::setProtcolInfo(const std::string & proto) throw(ClusterException)
{
    this->_protocolInfo = getprotobyname(proto.c_str());
    if (_protocolInfo == NULL) {
        std::cerr   << RED
                    << "function: getprotobyname()"
                    << RESET << std::endl;
        throw ClusterException().what();
    }
    return this->_protocolInfo;
}
