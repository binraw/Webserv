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
    controlLineOfFile();
    // if (!inputFile.is_open()) 
    // {
    //     std::cerr << "Could not open conf file" << std::endl;
    //     return;
    // }
    while (std::getline(inputFile, line))
    {
        if (line.empty() || line[0] == '#') continue;
        _allConf = UtilParsing::split(line, std::string(" "));
    }
    initDefaultConf();
    try 
    {
        controlParseFileConf();
    }
    catch (std::exception &e)
    {
        inputFile.close();
        throw;
    } 
    cleanClusterConfDefault();
    createMapDefaultConf();
    defaultParams.addValuesParams(_mapDefaultParamsCluster);
    defaultParams.printStructMapDefault();
    // printMapDefaultParamsCluster();
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
    // std::cout << controlParseFileConf() << std::endl;
    // cleanClusterConfDefault();
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


void Cluster::controlParseFileConf() throw(ErrorNumberOfBracket, ErrorBracketStick)
{
    int i = 0;
    int y = 0;
    std::string word;
    for(std::vector<std::string>::iterator it = _allConf.begin(); it != _allConf.end(); it++)
    {
        word = *it;
        if (*it == std::string("{"))
            i++;
        else if (*it == std::string("}"))
            y++;
        if (word.find("{") != std::string::npos || word.find("}") != std::string::npos)
        {
            if (word.length() > 1) 
                throw ErrorBracketStick();
        }
    }
    if (i == 0 || i != y)
        throw ErrorNumberOfBracket();
}


void Cluster::controlLineOfFile() throw(ErrorOpenFile, ErrorEndOfLine)
{
    std::ifstream inputFile(_configPath.c_str());
    std::string line;
    if (!inputFile.is_open())
        throw ErrorOpenFile();
    while (std::getline(inputFile, line))
    {
        if (line.empty() || line[0] == '#') continue;
        if (line.length() >= 1) 
        {
            char last = line[line.length() - 1];
            // std::cout << "le dernier caractère est : " << last << std::endl;
        if (last != ' ' && last != ';' && last != '{' && last != '}')
        {
            inputFile.close();
            throw ErrorEndOfLine();
        }
        }
    }
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


void Cluster::createMapDefaultConf()
{
    std::string key;
    std::string valuestr;
    std::string keystr;
    std::vector<std::string> params;

    for (std::vector<std::string>::iterator it = _defaultConf.begin(); it != _defaultConf.end(); ++it)
    {
        valuestr = *it;
        if (it != _defaultConf.begin())
        {
            std::vector<std::string>::iterator prev = it - 1;
            keystr = *prev;
            if (valuestr.find(';') == std::string::npos && keystr.find(';') != std::string::npos)
                key = *it;
        }
        else if (it == _defaultConf.begin() || valuestr.find(';') == std::string::npos)
        {
            key = *it;
        }
        for (std::vector<std::string>::iterator tic = it + 1; tic != _defaultConf.end(); tic++)
        {
            valuestr = *(tic - 1);
            keystr = *tic;
            if (tic->find(";") != std::string::npos)
            {
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
    }
}



void Cluster::printMapDefaultParamsCluster() const
{
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = _mapDefaultParamsCluster.begin(); 
         it != _mapDefaultParamsCluster.end(); 
         ++it)
    {
        const std::string& key = it->first;
        const std::vector<std::string>& params = it->second;

        std::cout << "Key: " << key << "\nParams: ";

        for (std::vector<std::string>::const_iterator paramIt = params.begin(); 
             paramIt != params.end(); 
             ++paramIt)
        {
            std::cout << *paramIt << " ";
        }

        std::cout << std::endl;
    }
}

const char* Cluster::ErrorNumberOfBracket::what() const throw()
{
    return "Error Parsing: not same number of '{' and '}'.";
}


const char* Cluster::ErrorBracketStick::what() const throw()
{
    return "Error Parsing: Bracket stick with word.";
}

const char* Cluster::ErrorEndOfLine::what() const throw()
{
    return "Error Parsing: End of line isn't valid.";
}

const char* Cluster::ErrorOpenFile::what() const throw()
{
    return "Error: File can't open.";
}

void s_DefaultParams::addValuesParams(std::map<std::string, std::vector<std::string> > mapRecover)
{
    for (std::map<std::string, std::vector<std::string> >::iterator it = mapRecover.begin(); it != mapRecover.end(); it++)
    {
        const std::string &key = it->first;
        const std::vector<std::string> &value = it->second;
        if (params.find(key) != params.end())
            params[key].insert(params[key].end(), value.begin(), value.end()); // rajout des values a la suite
        else
            params[key] = value; // la en gros si la key n'existe pas de base dans la map default on la rajoute
    }
}

void s_DefaultParams::printStructMapDefault()
{
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = params.begin(); 
         it != params.end(); 
         ++it)
    {
        const std::string& key = it->first;
        const std::vector<std::string>& value = it->second;

        std::cout << "Key: " << key << "\nParams: ";

        for (std::vector<std::string>::const_iterator paramIt = value.begin(); 
             paramIt != value.end(); 
             ++paramIt)
        {
            std::cout << *paramIt << " ";
        }

        std::cout << std::endl;
    }
}
