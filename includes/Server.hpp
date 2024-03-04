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
#include <vector>

#include "User.hpp"
#include "Pages.hpp"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class Server
{
    private:
        int                 fd;
        int                 port;
        std::string         host;
        std::string         server_name;
        std::string         root;
        std::string         index;
        std::string         error_page;
        std::string         client_max_body_size;
        std::string         directory_listing;
        
    public:
        std::vector<std::string> method;
        Server(void);
        ~Server(void);

        int setPort(int port);
        void setFd(int port);
        int setHost(std::string host);
        int setServerName(std::string server_name);
        int setRoot(std::string root);
        int setIndex(std::string index);
        int setErrorPage(std::string error_page);
        int setClientMax(std::string client_max_body_size);
        int setDirectory(std::string directory_listing);

        int         getPort(void);
        int         getFd(void);
        std::string getHost(void);
        std::string getServerName(void);
        std::string getRoot(void);
        std::string getIndex(void);
        std::string getErrorPage(void);
        std::string getClientMax(void);
        std::string getDirectory(void);
};

#endif