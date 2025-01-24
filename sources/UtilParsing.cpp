


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

std::vector<std::string> UtilParsing::cleanVector(std::vector<std::string> vec) {
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ) {
        if (*it == "{" || *it == "}") 
        { 
            it = vec.erase(it);
        } else 
            ++it;
    }
    return vec;
}


std::vector<std::string> UtilParsing::cleanVectorClose(std::vector<std::string> vec)
{
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ) {
        if (*it == "}") 
        { 
            it = vec.erase(it);
        } else 
            ++it;
    }
    return vec;
}

// void UtilParsing::printMapVector(std::map<std::string, std::vector<std::string> > vec)
// {
//     for (std::map<std::string, std::vector<std::string> >::iterator it = vec.begin(); 
//          it != vec.end(); 
//          ++it)
//     {
//         const std::string& key = it->first;
//         const std::vector<std::string>& params = it->second;

//         std::cout << "Key: " << key << "\nParams: ";

//         for (std::vector<std::string>::const_iterator paramIt = params.begin(); 
//              paramIt != params.end(); 
//              ++paramIt)
//         {
//             std::cout << *paramIt << " ";
//         }

//         std::cout << std::endl;
//     }
// }


void UtilParsing::printMapVector(const std::map<int, std::map<std::string, std::vector<std::string> > >& allMapRoads)
{
    // Itérateur pour la map de niveau supérieur
    for (std::map<int, std::map<std::string, std::vector<std::string> > >::const_iterator outerIt = allMapRoads.begin(); 
         outerIt != allMapRoads.end(); 
         ++outerIt)
    {
        int index = outerIt->first; // clé de niveau supérieur
        const std::map<std::string, std::vector<std::string> >& innerMap = outerIt->second;

        std::cout << "Index: " << index << std::endl;

        // Itérateur pour la map de niveau inférieur
        for (std::map<std::string, std::vector<std::string> >::const_iterator innerIt = innerMap.begin(); 
             innerIt != innerMap.end(); 
             ++innerIt)
        {
            const std::string& key = innerIt->first; // clé de niveau inférieur
            const std::vector<std::string>& params = innerIt->second;

            std::cout << "  Key: " << key << "\n  Params: ";

            // Itérateur pour le vecteur de chaînes
            for (std::vector<std::string>::const_iterator paramIt = params.begin(); 
                 paramIt != params.end(); 
                 ++paramIt)
            {
                std::cout << *paramIt << " ";
            }

            std::cout << std::endl;
        }
    }
}
