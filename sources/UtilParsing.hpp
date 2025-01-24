

/* 
    * class IParsing
    * Interface that brings together several static functions for string parsing
    * function list :
        -> split()
        -> 
 */

#ifndef IPARSING_HPP
# define IPARSING_HPP

# include <string>
# include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>

class UtilParsing
{
    public:
        static  std::vector<std::string> split(const std::string & line, const std::string & set); 
        static  bool isOnlySpace(const std::string & str);
        static  std::vector<std::string> cleanVector(const std::vector<std::string> vec);
        static  std::vector<std::string> cleanVectorClose(const std::vector<std::string> vec);
        static  void printMapVector(std::map<std::string, std::vector<std::string> > vec);
    private:
        UtilParsing() {};
};

#endif
