#include "Cluster.hpp"


Cluster::Cluster()
{
}

Cluster::~Cluster()
{
}

Cluster::Cluster(const std::string &filename)
{
    _configPath = filename;
    std::ifstream inputFile(filename.c_str());
    std::string line;
    if (!inputFile.is_open()) 
    {
        std::cerr << "Could not open conf file" << std::endl;
        return;
    }
    while (std::getline(inputFile, line))
    {
        if (line.empty() || line[0] == '#') continue;
        _allConf = UtilParsing::split(line, std::string(" "));
    }
    initDefaultConf();
    createMapDefaultConf();
    printMapDefaultParamsCluster();
    initAllServer();
    inputFile.close();
}

// Ici je recolte les servers que je mets dans une map 

void Cluster::initDefaultConf()
{
    std::vector<std::string>::iterator it;
    int i = 0;
    int number_servers = 0;
    for(it = _allConf.begin(); it != _allConf.end(); it++)
    {
        if (*it == std::string("{"))
            i++;
        if (*it == std::string("server"))
        {
            //faut utiliser pair pour creer des paire comme ca avec des maps tres style
            std::pair<int, std::vector<std::string> > serverPair(number_servers, addValuesServers(it));
            _vectServers.insert(serverPair);
            // _defaultConf.pop_back();  j enleve je sais pas si je casse tout
            number_servers++;
        }
        if (*it == std::string("}"))
            i--;
        if (i <= 1)
            _defaultConf.push_back(*it);
    }
    cleanClusterConfDefault();
    // for (std::vector<std::string>::iterator t = _defaultConf.begin(); t != _defaultConf.end(); t++)
    // {
    //     std::cout << *t << std::endl;
    // }

//   for (std::map<int, std::vector<std::string> >::const_iterator tic = _vectServers.begin(); tic != _vectServers.end(); ++tic) {
//         std::cout << "Server Number: " << tic->first << "\nConfigurations:\n";
        
//         // Itération sur le vecteur de configurations
//         for (std::vector<std::string>::const_iterator configIt = tic->second.begin(); configIt != tic->second.end(); ++configIt) {
//             std::cout << "  - " << *configIt << "\n"; // Affiche chaque configuration
//         }
//     }

}

std::vector<std::string> Cluster::addValuesServers(std::vector<std::string>::iterator &cursor)
{
    std::vector<std::string> server;
    std::vector<std::string>::iterator it;
    server.push_back(*cursor);
    int i = 0;
    int y = 0;
    for(it = cursor;it != _allConf.end(); it++)
    {
        if (*it == std::string("{"))
            i++;
        if (*it == std::string("}"))
            i--;
        if (i > 0)
            server.push_back(*it);
        if (i == 0 && y > 0)
            break;
        y++;
    }
    // std::cout << "Value du serveur :" << std::endl;
    // for (size_t y = 0; y < server.size(); y++) 
    // {
    //     std::cout << server[y] << std::endl;
    // }
    return server;
}

void Cluster::initAllServer()
{
    for(std::map<int, std::vector<std::string> >::const_iterator it = _vectServers.begin(); it != _vectServers.end(); it++)
    {
       _servers.push_back(Server(it->second));
    }
}

void Cluster::cleanClusterConfDefault()
{
    _defaultConf = UtilParsing::cleanVector(_defaultConf);
    for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); )
    {
        if (*it == "{" || *it == "}" || *it == "http" || *it == "server") 
        { 
            it = _defaultConf.erase(it);
        } else 
            ++it;
    }
}


// reussir a faire une map organiser des valeurs default de cluster 
// la trouver un algo pour check quand il y a un ; et si celui d'avant et encore avant en a 
// genre si jai pas jai pas et apres jai alors donner la key au premier 
// et creer une string composer des autres 
// doit etre modulable si y a 5 argument avec les methodes par exemple par la suite

// void Cluster::createMapDefaultConf()
// {
//     std::string key;
//     std::vector<std::string>::iterator tic;
//     size_t i = 0;
//     std::vector<std::string> params;
    
//     for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); )
//     {
//         if (it == _defaultConf.begin())
//         {
//             key = *it;
//             it++;
//         }
//         tic = it--;
//         if (*tic.find(";"))
//         {
//             key = *tic;
//             if (params)
//                 std::pair<std::string,std::vector<std::string> > serverPair(key, *params);
//                 _mapDefaultParamsCluster.insert(serverPair);
//         }
//         else
//         {
//             params.push_back(*it);
//         }
//         it++;
//     }
// }



// void Cluster::createMapDefaultConf()
// {
//     std::string key;
//     std::vector<std::string> params;

//     for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); ++it)
//     {
//         // Si c'est le premier mot, le définir comme clé
//         if (it == _defaultConf.begin())
//         {
//             key = *it; // Initialiser la clé
//             continue;
//         }

//         // Si l'élément contient un point-virgule
//         if (it->find(";") != std::string::npos)
//         {
//             // Ajoutez les paramètres accumulés à la map
//             if (!params.empty())
//             {
//                 std::pair<std::string, std::vector<std::string> > serverPair(key, params);
//                 _mapDefaultParamsCluster.insert(serverPair);
//                 params.clear(); // Réinitialiser les paramètres pour le prochain groupe
//             }

//             // Définir la clé comme le mot juste avant le point-virgule
//             key = it->substr(0, it->find(";")); // Obtenir le mot avant le point-virgule
//             key.erase(0, key.find_first_not_of(" ")); // Supprimer les espaces au début

//             // Récupérer les paramètres après le point-virgule
//             std::string remainingParams = it->substr(it->find(";") + 1);
//             std::istringstream iss(remainingParams);
//             std::string param;

//             // Ajouter les paramètres restants
//             while (iss >> param) {
//                 params.push_back(param);
//             }
//         }
//         else
//         {
//             params.push_back(*it); // Ajouter tous les mots comme paramètres
//         }
//     }

//     // Gérer le dernier groupe de paramètres si nécessaire
//     if (!params.empty() || !key.empty())
//     {
//         std::pair<std::string, std::vector<std::string> > serverPair(key, params);
//         _mapDefaultParamsCluster.insert(serverPair);
//     }
// }

void Cluster::createMapDefaultConf()
{
    std::string key;
    std::string valuestr;
    std::vector<std::string> params;

    for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); ++it)
    {
        // Vérifiez si l'élément actuel contient un point-virgule
        valuestr = *it;
        if (it == _defaultConf.begin() || valuestr.find(';') == std::string::npos)
        {
            key = *it;

            // std::pair<std::string, std::vector<std::string> > serverPair(key, params);
            // _mapDefaultParamsCluster.insert(serverPair);
            // params.clear();
        }
        // if (it->find(";") != std::string::npos)
        // {
            // if (it != _defaultConf.begin()) 
            // {
            //     key = *(it - 1);
            // }
        for (std::vector<std::string>::iterator tic = it + 1; tic != _defaultConf.end(); tic++)
        {
            valuestr = *(tic - 1);
            if (tic->find(";") != std::string::npos)
            {
                // std::cout << "Adding parameter: " << *tic << std::endl;
                params.push_back(*tic);
            }
            else if (valuestr.find(";") == std::string::npos)
            {
                params.push_back(*tic);
            }
            else
                break;
        }
            std::pair<std::string, std::vector<std::string> > serverPair(key, params);
            _mapDefaultParamsCluster.insert(serverPair);
            params.clear();
        // }
    }
}



void Cluster::printMapDefaultParamsCluster() const
{
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = _mapDefaultParamsCluster.begin(); 
         it != _mapDefaultParamsCluster.end(); 
         ++it)
    {
        const std::string& key = it->first;  // Clé
        const std::vector<std::string>& params = it->second;  // Valeurs (vecteur de chaînes)

        std::cout << "Key: " << key << "\nParams: ";

        for (std::vector<std::string>::const_iterator paramIt = params.begin(); 
             paramIt != params.end(); 
             ++paramIt)
        {
            std::cout << *paramIt << " ";  // Affichage des paramètres
        }

        std::cout << std::endl;
    }
}
