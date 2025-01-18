



#ifndef ASERVER_HPP
# define ASERVER_HPP

#include <map>
#include <string>
#include <vector>

class AServer
{
    protected:
        AServer(std::map<std::string, std::vector<std::string>> & ref); // prend en parametre une instance de map qui correspond aux parametres de son cluster
        

    private:
        virtual ~AServer() {};
};

#endif