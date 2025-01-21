
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>

typedef struct s_serverDefault
{
	std::map<std::string, std::vecor<std::string> >
		defaultParams;
		
	s_serverDefault()
	{
		defaultParams =
		{
			{ "listen", { "8000" } },
			{ "server_name", { "localhost" } },
			{ "indexFiles", { "index.html" } },
			{ "defaultErrorPage", {"/error_pages"} },
			{ "defaultRoot", { "./websites" } }
			// a completer au fil du developpement
		};
	}
}	t_serverDefault;

typedef struct s_location {
    std::string path;                     // Chemin (ex: "/")
    std::string root;                     // Racine des fichiers (ex: "./html")
    std::vector<std::string> indexFiles;  // Fichiers d’index (ex: ["index.html", "index.htm"])
    bool autoindex;                       // Activer/désactiver l’autoindex
    std::map<int, std::string> errorPages; // Pages d’erreur spécifiques (404 -> "/404.html")

    Location()
      :	path("/")
	  	root("./websites"),
		indexFiles( { "index.html" } ),
		autoindex(false),
		errorPages( { "40x", { "error_pages/40x/" } } )
	{	} // Initialisation par défaut
}	t_location;


class Server {
	public:
		Server();
		~Server();

		void		setListenPorts(const std::vector<int> & ports);
		void		setServerNames(const std::vector<std::string> & names);
		t_location	& addLocation(t_location & ref);

		friend std::ostream &operator<<(std::ostream &, const Server &);

	private:
		t_serverDefault				_serverDefault;
		std::vector<int>			_listenPorts;
		std::vector<std::string>	_serverNames;
		std::vector<t_location>		_locations;
		
		// Sockets associés au serveur
        std::set<int>	_socketFDs;
};

#endif

