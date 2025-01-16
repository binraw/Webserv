

#define PATH_DEFAULTSERV    "./config/default.conf"
#define PATH_DEBUGSERV      "./config/debug.conf"
#define PATH_MIME           "./config/mime.types"

#include <string>
#include <iostream>

int main (int argc, char **argv)
{
    if (argc > 2){
        std::cout   << "USAGE :\n"
                    << "    -> a path to a file config (.conf)\n"
                    << "    -> no argument and default server is launch"
                    << std::endl;
        return (1);
    }
/*     
#ifdef (DEBUG)
    // lauch server with debug_file.conf
    // Cluster main(PATH_DEBUGSERV)
#else
    // lauch server with default_server.conf
    // Cluster main(PATH_DEFAULTSERV)
#endif
 */
    return (0);
}