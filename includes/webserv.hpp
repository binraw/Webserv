
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "colors"

# ifdef TEST
#  define TEST
# endif 

/* -------------------------------------- */
/* --- DEFINE GENERIC ERROR MESSAGES  --- */
/* -------------------------------------- */
#define ERROR	   "Error" << std::endl
#define PRINTUSAGE  std::cout   << YELLOW "USAGE :" << std::endl \
						<< "	-> a path to a file config (.conf)" << std::endl \
						<< "	-> no argument and default server is launch" \
						<< RESET << std::endl

/* -------------------------------------- */
/* ---		  DEFINE PATH		   --- */
/* -------------------------------------- */
#define PATH_DEFAULTSERV	"./config/default.conf"
#define PATH_DEBUGSERR		"./config/debug.conf"
#define PATH_ERRPAGE		"./error_pages"
#define PATH_MIME			"./config/mime.types"


#endif