

/* 
    * class IParsing
    * Interface that brings together several static functions for string parsing
    * function list :
        -> split()
        -> 
 */

#ifndef UTILPARSING_HPP
# define UTILPARSING_HPP

# include <string>
# include <vector>
# include <map>
# include <set>

class UtilParsing
{
    public:
        static std::vector<std::string>
                split(const std::string & line, const std::string & set); 
        static bool
                isOnlySpace(const std::string & str);
        static std::vector<std::string>
                cleanVector(const std::vector<std::string> vec);
        
        template <typename T, typename Y>
        static bool isKeyExist(const std::map<T, Y> & map, const T & key);

        template <typename T, typename Y>
        static bool	isvalueExist(const std::map<T, std::set<Y> > & map, const T & key, const Y & value);

    private:
        UtilParsing() {};
};

#endif
