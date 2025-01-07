# Webserv


## Fonctions de Système et de Réseau:

execve : Remplace le processus actuel par un programme spécifié, en exécutant le fichier binaire à l'emplacement donné. Prend le chemin du fichier, les arguments et les variables d'environnement.
dup : Duplique un descripteur de fichier, créant un nouveau descripteur qui référence le même fichier.
dup2 : Duplique un descripteur de fichier vers un autre descripteur spécifié. Si le descripteur cible existe, il est fermé avant la duplication.
pipe : Crée un canal de communication entre processus en établissant un tableau de deux descripteurs de fichier, permettant la communication unidirectionnelle.
strerror : Renvoie un pointeur vers une chaîne de caractères décrivant l'erreur correspondant à un code d'erreur donné.
gai_strerror : Renvoie une chaîne de caractères décrivant une erreur liée à la résolution de noms d'hôtes (gai), utilisant un code d'erreur de type gai_error.
errno : Variable globale qui contient le code d'erreur de la dernière fonction de système qui a échoué.
fork : Crée un nouveau processus en dupliquant le processus appelant. Le nouveau processus est appelé le "processus fils".
socketpair : Crée une paire de sockets connectés pour la communication entre processus, généralement utilisée pour la communication inter-processus.


## Fonctions de Conversion de Réseau:

htons : Convertit un nombre de 16 bits de l'ordre d'octets hôte à l'ordre d'octets réseau.
htonl : Convertit un nombre de 32 bits de l'ordre d'octets hôte à l'ordre d'octets réseau.
ntohs : Convertit un nombre de 16 bits de l'ordre d'octets réseau à l'ordre d'octets hôte.
ntohl : Convertit un nombre de 32 bits de l'ordre d'octets réseau à l'ordre d'octets hôte.


### Pourquoi ces conversions ?
Lorsque vous communiquez sur un réseau, les données doivent être interprétées de manière cohérente entre les différents systèmes.
Ces fonctions garantissent que les entiers que vous envoyez et recevez sont correctement interprétés, quel que soit l'ordre des octets de l'architecture sous-jacente.
Cela permet d'éviter des erreurs de communication dues à des différences dans la représentation des données.


## Fonctions de Sélection et de Polling:

select : Permet de surveiller plusieurs descripteurs de fichiers pour voir s'ils sont prêts pour une opération de lecture ou d'écriture.
poll : Semblable à select, mais utilise une structure de données différente pour surveiller les descripteurs de fichiers, permettant de gérer un plus grand nombre de descripteurs.
epoll_create : Crée un nouvel objet de gestion d'événements pour surveiller plusieurs descripteurs de fichiers, utilisé dans le modèle d'entrées/sorties asynchrone.
epoll_ctl : Contrôle l'objet epoll en ajoutant, modifiant ou supprimant des descripteurs de fichiers à surveiller.
epoll_wait : Attend des événements sur les descripteurs de fichiers surveillés par l'objet epoll et renvoie les descripteurs qui ont des événements prêts.

### En resume 
Les fonctions de sélection et de polling sont utilisées pour gérer l'entrée/sortie (E/S) dans les applications réseau et les systèmes interactifs.
Elles permettent de surveiller plusieurs descripteurs de fichiers (comme les sockets) pour déterminer lesquels sont prêts pour une opération d'E/S.


## Fonctions de Kqueue:

kqueue : Crée une nouvelle file d'attente d'événements pour surveiller des événements sur des descripteurs de fichiers ou d'autres événements du système.
kevent : Modifie et récupère des événements de la file d'attente d'événements créée par kqueue, permettant de gérer des événements tels que des connexions ou des lectures/écritures disponibles.


## Fonctions de Sockets:

socket : Crée un nouveau socket, qui est un point de communication pour l'envoi et la réception de données.
accept : Accepte une connexion entrante sur un socket d'écoute, créant un nouveau socket pour communiquer avec le client.
listen : Met un socket en mode écoute pour les connexions entrantes, spécifiant le nombre maximum de connexions en attente.
send : Envoie des données sur un socket.
recv : Reçoit des données sur un socket.
bind : Lie un socket à une adresse locale et à un port, permettant aux clients de se connecter à ce socket.
connect : Établit une connexion à un socket distant.
getaddrinfo : Résout un nom d'hôte ou une adresse pour obtenir des informations sur les adresses, comme les adresses IPv4 ou IPv6.
freeaddrinfo : Libère la mémoire allouée pour les informations d'adresse retournées par getaddrinfo.
setsockopt : Définit des options sur un socket, telles que la taille de la file d'attente ou le mode de réutilisation des adresses.
getsockname : Récupère l'adresse locale et le port d'un socket.
getprotobyname : Renvoie le numéro de protocole associé à un nom de protocole (comme "TCP" ou "UDP").


## Fonctions de Contrôle de Fichier:

fcntl : Modifie les attributs d'un descripteur de fichier, comme le mode non-bloquant.
close : Ferme un descripteur de fichier, libérant ainsi les ressources associées.
read : Lit des données à partir d'un descripteur de fichier.
write : Écrit des données sur un descripteur de fichier.


## Fonctions de Gestion de Processus:

waitpid : Attend qu'un processus fils se termine et récupère son statut.
kill : Envoie un signal à un processus spécifié, généralement utilisé pour terminer un processus.
signal : Définit un gestionnaire de signal pour un signal donné, permettant de gérer des événements asynchrones.


## Fonctions de Système de Fichiers:

access : Vérifie les permissions d'accès à un fichier.
stat : Récupère les informations sur un fichier, comme sa taille, ses permissions et ses timestamps.
open : Ouvre un fichier et renvoie un descripteur de fichier associé.
opendir : Ouvre un répertoire et retourne un pointeur vers une structure de répertoire.
readdir : Lit une entrée dans un répertoire ouvert, renvoyant le nom du fichier.
closedir : Ferme un répertoire ouvert, libérant les ressources associées.


## Infos recoltees sur different site:

### (peut etre la facon de faire pour le projet)
Historiquement, Apache fonctionne en prefork, ce qui signifie qu'un processus père lancé avec des droits étendus (root) 
démarre des processus enfants qui traiteront chacun un certain nombre de requêtes clients. Cependant, sous Linux, 
la multiplication des processus provoque une augmentation de consommation de ressources (mémoire, descripteurs de fichiers).


## MACROS:

FD_SET: Ajoute le descripteur de fichier fd à l'ensemble set.
FD_ZERO: Initialiser l'ensemble à vide.
FD_CLR: Supprime le descripteur de fichier fd de l'ensemble set.
FD_ISSET: Verifie si le descripteur de fichier fd est present dans l'ensemble sinon zero.


