


#include "IParsing.hpp"

/*
    exemple d'une fonction split bidon pour l'exemple
*/

std::string **  split(std::string & line, std::string & set)
{
    /*
        exemple alloc clean en c++
        l'operateur new() leve une exeception std::bad_alloc
    */
    std::string **  splitResult;

    try {
        splitResult = new std::string *;
    }
    catch(const std::bad_alloc & e) {
        std::cerr   << RED 
                    << "error : split() at line [" << __LINE__ << "]\n"
                    << RESET
                    << e.what() << '\n';
        return NULL;
    }
    try {
        splitResult[0] = new std::string("coucou");
    }
    catch(const std::exception& e)
    {
        std::cerr   << RED 
                    << "error : split() at line [" << __LINE__ << "]\n"
                    << RESET
                    << e.what() << '\n'
                    << e.what() << '\n';
        delete splitResult;
        return NULL;
    }
    return splitResult;
}
