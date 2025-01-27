


#include "UtilParsing.hpp"

template <typename T, typename Y>
bool	UtilParsing::isKeyExist(const std::map<T, Y> & map, const T & key) {
	return map.find(key) != map.end();
}

template <typename T, typename Y>
bool	UtilParsing::isvalueExist(const std::map<T, std::set<Y> > & map, const T & key, const Y & )
{
	typename std::map<T, std::set<Y> >::const_iterator	it = map.find(key);
	if (it == map.end())
		return false;
	return true; // HAVE TO CHECK THIS FUNCTION
}

bool    UtilParsing::isOnlySpace(const std::string & str)
{
    size_t  i;
    size_t  size = str.size();

    for (i = 0; i < size; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            return (i == size);
        }
    }
    return (i == size);
}

std::vector<std::string> UtilParsing::split(const std::string & line, const std::string & set)
 {
    static std::vector<std::string> result;

    for (size_t i = 0; i != std::string::npos;) {
        size_t end = line.find_first_of(set, i);
        result.push_back(line.substr(i, end - i));
        if (result.back().empty() || UtilParsing::isOnlySpace(result.back()))
            result.pop_back();
        end != std::string::npos ? i = end + 1 : i = end;
    }

    return result;
}

std::vector<std::string> UtilParsing::cleanVector(std::vector<std::string> vec)
{
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ) {
        if (*it == "{" || *it == "}") 
        { 
            it = vec.erase(it);
        } else 
            ++it;
    }
    return vec;
}
