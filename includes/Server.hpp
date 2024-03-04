#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>
#include <map>
#include <vector>

#include "User.hpp"
#include "Server.hpp"
#include "Pages.hpp"

// #ifndef PORT
// #define PORT 8080
// #endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

extern int		g_exit;

class Server
{
    private:

        int                 fd;
        int                 rc;
        int                 max_sd;
        int                 new_sd;
        int                 len;
        int                 end_server;
        struct sockaddr_in6 address;
        struct timeval      timeout;
        fd_set	            readfds;
        fd_set	            writefds;
        // pourquoi ne pas set des errorfds pour les fd a surveiller pour les erreurs
        fd_set	            tmp_readfds;
        fd_set	            tmp_writefds;
        char                buffer[BUFFER_SIZE];

        int port;
        std::string host;
        std::string server_name;
        std::string root;
        std::string index;
        std::string error_page;
        std::string client_max_body_size;
        std::string directory_listing;

        void	errorFunction(std::string word);
        int		readServer(int i);
        void	sendServer(int i);
        void    listenServer(Server server);

    public:
        int initServer(Server server);
        int runServer(void);
        Server(void);
        ~Server(void);

        void setPort(int port);
        void setFd(int port);
        void setHost(std::string host);
        void setServerName(std::string server_name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setErrorPage(std::string error_page);
        void setClientMax(std::string client_max_body_size);
        void setDirectory(std::string directory_listing);
        int initializeSets(void);

        int         getPort(void);
        int         getFd(void);
        std::string getHost(void);
        std::string getServerName(void);
        std::string getRoot(void);
        std::string getIndex(void);
        std::string getErrorPage(void);
        std::string getClientMax(void);
        std::string getDirectory(void);

        std::map<int, User> Users;
        std::map<int, Server> Servers;
        int nb_servers;
};

#endif