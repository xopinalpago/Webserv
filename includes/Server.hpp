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

#include "Utils.hpp"

class Server
{
    private:
        int                         fd;
        int                         port;
        unsigned int                client_max_body_size;
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

        int 	                    setPort(int port);
        int 	                    setFd(int fd);
        int 	                    setHost(std::string host);
        int 	                    setServerName(std::string server_name);
        int 	                    setRoot(std::string root);
        int 	                    setIndex(std::string index);
        unsigned int                setClientMax(unsigned int client_max_body_size);
        int 	                    setDirectory(std::string directory_listing);
        void 	                    setMethod(std::string tmp);
        void 	                    setErrorPage(int key, std::string tmp);
        void 	                    setCgiEx(std::string tmp);
 
        int         				getPort(void) const;
        int         				getFd(void) const;
        unsigned int        		getClientMax(void) const;
        in_addr_t    				getHost(void) const;
        std::string 				getServerName(void) const;
        std::string 				getRoot(void) const;
        std::string 				getIndex(void) const;
        std::string 				getDirectory(void) const;
        std::vector<std::string> 	getMethod(void) const;
        std::string 				getMethodi(int i) const;
        std::vector<std::string>    getCgiEx(void) const;
        std::string                 getCgiExi(int i) const;
        std::map<int, std::string>& getErrorPage(void);
        std::string                 getErrorPagei(int i);
};

#endif