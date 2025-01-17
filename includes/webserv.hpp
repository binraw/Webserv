
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "colors"

# ifdef TEST
#  define TEST
# endif 

/* -------------------------------------- */
/* --- DEFINE GENERIC ERROR MESSAGES  --- */
/* -------------------------------------- */
#define ERROR       "Error\n"
#define PRINTUSAGE  std::cout   << YELLOW "USAGE :\n" \
                        << "    -> a path to a file config (.conf)\n" \
                        << "    -> no argument and default server is launch" \
                        << RESET << std::endl

/* -------------------------------------- */
/* ---          DEFINE PATH           --- */
/* -------------------------------------- */
#define PATH_DEFAULTSERV    "./config/default.conf"
#define PATH_DEBUGSERV      "./config/debug.conf"
#define PATH_MIME           "./config/mime.types"




#endif