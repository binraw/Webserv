


#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include "webserv.hpp"

#include <map>
#include <string>
#include <vector>


class AServer;
class ClusterException;

class Cluster
{
    public:
        Cluster(const std::string & file) throw(ClusterException);
        Cluster(const Cluster &);
        ~Cluster();
        
        Cluster &               operator=(const Cluster &) const;
        friend std::ostream &   operator<<(std::ostream &, const Cluster &);
    
    private:
        static std::map<std::string, std::vector<std::string>>
                _clusterParams;
        /*
            contient tous les parametres par defaut si aucun bloc protocole n'est defini
        */
        
        std::vector<std::string>
                & _splitedFile;    // contient tout le fichier de config renvoye par split
        /*
            vecteur renvoye par split()
        */
        std::map<std::string, std::vector<std::string>>
                & _clusterBlock; // contient tout les blocs protocole
        // parser ce bloc pour mettre a jour le tableau static _clusterParams
        // en sortie de ce parsing on retourne une nouvelle map destines a etre des params des serveurs
        

        std::map<std::string, std::vector<AServer*>>
                _service_servers;
        /*  exemple de structure attendue
            _service_servers {
                {"http", {AServer1*, AServer2*, AServer3*}},
                {"https", {AServer1*, AServer2*, AServer3*}}
            }
        */
        /*
            infos necessaires avant initialisation des serveurs :
                - le nb de protocoles differents (HTTP HTTPS IRC etc)
                - le nb de serveur virtuelles a initialiser par protocoles

            il me faut une liste de parametres par protocole
                pour modifier les parametres static du tableau de params de Cluster
                un tableau de params par protocoles (si plusieurs sont pris en charge, renvoie une erreur si le protocole n'est pas pris en charge (ex HTTPS))

            
            il me faut une liste de parametre pour chaque serveur du protocole


            1 vecteur de map 
        */
        // MEMBERS //


        // EXCEPT CLASS //
        class ClusterException : std::exception
        {
            public:
                const char *    what() const throw();
                void            display(std::string & msg) const throw();

        }


} ;

#endif