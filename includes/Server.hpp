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

#include "User.hpp"
#include "Cgi.hpp"

#ifndef PORT
#define PORT 8080
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

extern int		g_exit;

class Server
{
    private:
        int                 server_fd;
        int                 rc;
        int                 max_sd;
        int                 new_sd;
        int                 end_server;
        struct sockaddr_in6 address;
        struct timeval      timeout;
        fd_set	            readfds;
        fd_set	            writefds;
        fd_set	            tmp_readfds;
        fd_set	            tmp_writefds;

        void	errorFunction(std::string word);
        int		readServer(int i);
        void	sendServer(int i);
        void    listenServer(void);

    public:
        int initServer(void);
        int runServer(void);
        Server(void);
        ~Server(void);
        std::map<int, User> Users;

};

#endif