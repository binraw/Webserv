
/*
	FICHIER DE TEST
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <string>
#include <set>

#include <cstring>
#include <iostream>

/*
	* int socket(int __domain, int __type, int __protocol)
		* cree un nouveau socket de communication.
		
	* int __domain
	* AF_INET ->		domaine IPV4
		
	* int __type
	* SOCK_STREAM ->	le transfert de donnees entre l'interface reseau et l'application 
							se fait par flux de donnee et non par datagramme ( TCP )
	* SOCK_NONBLOCK ->  le socket est non bloquant (voir ce que cela implique plus tard)
							d'apres la doc, evite d'appeler fcntl() de maniere abusive
		
	* int __protocole
	* 
*/
/*
	* int setsockopt(int __fd, int __level, int __optname, const void *__optval, socklen_t __optlen)
		la particularrite de cette fonction c'est que les options ne peuvent pas etre chainees par un |
		il faut appeler la fonction autant de fois qu'il faut set un parametre
	* setsockopt() est une fonction qui permet un parametrage avance sur differentes
		couches du protocole de communication. 
	* le niveau de la couche sur laquel on agit est definit dans le champ level, ainsi on peut parametrer :
		-> au niveau de la couche application, (API socket, globalement la gestion du fd par l'os).
		-> au niveau de la couche transport (parametres specifique au protocole TCP ou UDP suivant celui utilise par le socket)
		-> au niveau de la couche reseau ( parametres specifique au protocole IP )
*/


int main (int argc, char **argv)
{
	
		

	// client_addr_size = sizeof(client_addr);
	// fd_client = accept(fd_sock_server, (struct sockaddr *) &client_addr, &client_addr_size); // accepte la connexion et creer un nouveau socket
	// if (fd_client < 0) {
	//	 std::cerr << "Error accept client" << std::endl;
	//	 freeaddrinfo(res);
	// }

	// memset(buffer, '\0', sizeof(buffer));
	// bytes_received = recv(fd_client, buffer, sizeof(buffer) - 1, 0);
	
	// if (bytes_received < 0) 
	// {
	// 	std::cerr << "Erreur lors de la réception des données" << std::endl;
	// 	close(fd_client);
	// 	freeaddrinfo(res);
	// 	return (5);
	// }
	// const char *http_response =
	// "HTTP/1.1 200 OK\r\n"
	// "Content-Type: text/html\r\n"
	// "Content-Length: 13\r\n"
	// "\r\n"
	// "Hello, World!";

	// if (send(fd_client, http_response, strlen(http_response), 0) < 0) 
	// {
	// 	std::cerr << "Erreur lors de l'envoi de la réponse" << std::endl;
	// 	close(fd_client);
	// 	freeaddrinfo(res);
	// 	return (6);
	// }

	// std::cout << http_response << std::endl;

	// close(fd_client);
	// std::cout << "New client : " << client_addr.ss_family << std::endl;
	// close(fd_sock_server);
	// freeaddrinfo(res);

	return (0);
}