
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

typedef struct s_serverDefault
{
	std::map<std::string, std::vector<std::string> >
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

typedef struct s_location
{
    std::string					path;		// Chemin (ex: "/")
    std::string					root;		// Racine des fichiers (ex: "./html")
    std::vector<std::string>	indexFiles;	// Fichiers d’index (ex: ["index.html", "index.htm"])
    bool						autoindex;	// Activer/désactiver l’autoindex
    std::map<int, std::string>	errorPages;	// Pages d’erreur spécifiques (404 -> "/404.html")

    s_location()
      :	path("/"),
	  	root("./websites"),
		indexFiles( { "index.html" } ),
		autoindex(false),
		errorPages( { "40x/", { "error_pages/40x/" } } )
	{	}
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
		t_serverDefault				_serverParams;	// parametres du serveur ( mis a jour au parsing )
		std::vector<int>			_listenPorts;	// liste des ports sur lesquel le serveur doit ecouter
		std::vector<std::string>	_serverNames;	// liste des noms reconnus par le serveur
		std::vector<t_location>		_locations;		// liste des locations du serveur
		
		// Sockets associés au serveur. std::set est un objet qui contient des donnes uniques (il ne peut pas y avoir deux fois le meme int)
        std::set<int>	_socketFDs;
};

#endif
