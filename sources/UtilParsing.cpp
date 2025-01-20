


#include "UtilParsing.hpp"

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


    // for (size_t i = 0; i < result.size(); i++) {
    //     std::cout << result[i] << std::endl;
    // }


    return result; // Now returning by value
}

