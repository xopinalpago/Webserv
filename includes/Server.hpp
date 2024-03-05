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
#include <map>
#include <arpa/inet.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

// class User ;

class Server
{
    private:
        int                         fd;
        int                         port;
        int                         client_max_body_size;
        in_addr_t                   host;
        std::string                 server_name;
        std::string                 root;
        std::string                 index;
        std::string                 directory_listing;
        std::vector<std::string>    method;
        std::vector<std::string>    cgi_extension;
        std::map<int, std::string>  error_page;
        
    public:
        Server(void);
        ~Server(void);

        struct sockaddr_in address;
        
        int 	setPort(int port);
        void 	setFd(int port);
        int 	setHost(std::string host);
        int 	setServerName(std::string server_name);
        int 	setRoot(std::string root);
        int 	setIndex(std::string index);
        int 	setClientMax(int client_max_body_size);
        int 	setDirectory(std::string directory_listing);
        void 	setMethod(std::string tmp);
        void 	setErrorPage(int key, std::string tmp);
        void 	setCgiEx(std::string tmp);

        int         				getPort(void);
        int         				getFd(void);
        int        					getClientMax(void);
        in_addr_t    				getHost(void);
        std::string 				getServerName(void);
        std::string 				getRoot(void);
        std::string 				getIndex(void);
        std::string 				getDirectory(void);
        std::vector<std::string> 	getMethod(void);
        std::string 				getMethodi(int i);
        std::vector<std::string>    getCgiEx(void);
        std::string                 getCgiExi(int i);
        std::map<int, std::string>  getErrorPage(void);
        std::string                 getErrorPagei(int i);
};

#endif