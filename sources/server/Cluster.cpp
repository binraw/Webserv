


/*============================================================================*/
                        /*### HEADER FILES ###*/
/*============================================================================*/

#include "Cluster.hpp"


/*============================================================================*/
                    /*### INITIALISATION STATIC FIELD ###*/
/*============================================================================*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                    /*### CONSTRUCTORS (DEFAULT & COPY) ###*/
/*============================================================================*/

Cluster::Cluster(const std::string & filepath) throw(ClusterException)
  : _protocolInfo(this->setProtcole("tcp"))
{


}
/*----------------------------------------------------------------------------*/

Cluster::Cluster(const Cluster & ref)
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### DESTRUCTORS ###*/
/*============================================================================*/

Cluster::~Cluster()
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                    /*### OVERLOAD OPERATOR ###*/
/*============================================================================*/

Cluster & Cluster::operator=(const Cluster & ref) const
{   }
/*----------------------------------------------------------------------------*/

std::ostream & operator<<(std::ostream & o, const Cluster & right)
{   }
/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### GETTER - SETTER ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PRIVATE METHODS ###*/
/*============================================================================*/
/*
    * protoent *getprotobyname(const char *__name)

        * revoie une structure protoent qui contient:
        * char  *name -> chaine de caractere avec le nom du protocole demande (ici il sera egale a "tcp")
        * char  **alias -> tableau de chaine de caractere qui contient tousles alias du protocole (exemple alias[0] = "TCP")
        * int   num_proto -> le numero de protocole qui sera donne en dernier parametre de la fonction socket()
            afin de l'initialiser proprement et rendre le code fonctionnel sur plusieurs plateformes (WINDOWS LINUX ETC...)
*/
protoent *      Cluster::setProtcole(const std::string & proto) throw(ClusterException)
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
/*----------------------------------------------------------------------------*/

std::map<std::string, std::string>  & setInitInfo(const std::string & file) \
    throw(ClusterException)
{
    /*
        parsing fichier;
        remplir un model std::map qui contient tout les champs a extraire du fichier
        leve une exception si une erreur de parsing est detectee
    
        model std::map :
            - doit contenir le nombre de serveurs virtuel
    */
}
/*----------------------------------------------------------------------------*/

std::map<Server, int>               & setVirtualServer()
{


}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/

/*============================================================================*/
                        /*### PUBLIC METHODS ###*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/*============================================================================*/
                            /*### EXCEPTIONS ###*/
/*============================================================================*/

const char *    Cluster::ClusterException::what() const throw() {
    return (ERROR"During Cluster initilization\n");
}
/*----------------------------------------------------------------------------*/

void            Cluster::ClusterException::display(std::string & msg) const throw()
{
    std::cerr   << RED ERROR
                << msg
                << RESET << std::endl;
}
/*----------------------------------------------------------------------------*/
