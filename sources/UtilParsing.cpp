
#ifdef TEST
#endif

#include "UtilParsing.hpp"

#include <iostream>

bool
	UtilParsing::isOnlySpace(const std::string & str)
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

std::vector<std::string>
    & UtilParsing::split(const std::string & line, const std::string & set)
{
    std::vector<std::string>    result;
    
    for (size_t i = 1; i != std::string::npos;) {
        size_t end = line.find_first_of(set, i);
        result.push_back(line.substr(i, end - i));
        if (result.back().empty() == true \
            || UtilParsing::isOnlySpace(result.back()) == true)
            result.pop_back();
        end != std::string::npos ? i = end + 1 : i = end;
    }
#ifdef TEST
    for (size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] << std::endl;
    }
#endif
    return result;
}

/*
	- params 1 : container avec tous les tokens ou doit se faire la recherche
	- params 2 : a partir de quel index commencer la recherche
					ce parametre doit pointer sur le bracket sur lequel on doit chercher
	- return 0 si il n'y a pas de bracket fermant associe
*/
size_t
	UtilParsing::findBracketClosing(const std::vector<std::string> & container, size_t i_start)
{
	int	count = 1;

	if (container[i_start].compare("{") != 0) {
		std::cerr	<< RED ERROR
					<< "function : findBracketClosing()" << std::endl
					<< "-> i_start doesn't point on the good value, expected \"{\" but got \""
					<< container[i_start] << "\""
					<< RESET << std::endl;
		return 0;
	}
	i_start++;
	for (i_start; i_start < container.size(); i_start++) {
		if (container[i_start].compare("{") == true)
			count++;
		if (container[i_start].compare("}") == true)
			count--;
		if (count == 0)
			return (i_start);
	}
	return (0);
}