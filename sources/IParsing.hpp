

/* 
    * class IParsing
    * Interface that brings together several static functions for string parsing
    * function list :
        -> split()
        -> 
 */

#ifndef IPARSING_HPP
# define IPARSING_HPP

# include "webserv.hpp"
# include <string>

class IParsing
{
    private:
        virtual ~IParsing() = 0;
    
    public:
        // prototype pour l'exemple, a modifier selon besoin
        static std::string **   split(std::string & line, std::string & set); 
};

#endif