




#ifndef POSTREQUEST_HPP
# define POSTREQUEST_HPP

# include "webserv.hpp"
# include "ARequest.hpp"
#include "Server.hpp"
#include <sys/wait.h>
#include <fstream>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

class PostRequest : virtual public ARequest
{
	public:
		PostRequest(const std::string &);
		PostRequest(const PostRequest &);
		void executePostRequest(); // ici ca sera une fct propre a ARequest et seulement sont content diff.
		~PostRequest();
		PostRequest &operator=(const PostRequest &);
		std::string playCGI(); 
		std::string playUploadCGI();
		void save_file(const std::string& request_body);
		std::string extract_filename(const std::string& request_body);

	private:
		// std::vector<std::string>	_arguments; ne pas dissocier les arguments je pense que renvoyer le content
		// est plus utile car le perl permet de recuperer grace aux balises.
		// apres a voir car parfois la requete ne mets pas forcement les balises et seulement
		// des '=' si c'est le cas alors je change le script.
		std::string					_contentReq;
		std::string					_pathCGI; // ici il faut me donner le path complet exemple : /cgi-bin/script.pl
		std::string					_contentFileUpdate;
		std::string					_contentFileForUpload; // ici voir si il faut le fichier (je pense oui)
		// ou si il suffit de tout avoir dans content et avec le chemin copier le content dans le 'nouveau' fichier



};

std::ostream & operator<<(std::ostream &, const PostRequest&);

#endif
