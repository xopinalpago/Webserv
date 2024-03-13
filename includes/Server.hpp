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
#include "Location.hpp"

class Server
{
    private:
        int                         fd;
        int                         port;
        int                         client_max_body_size;
        in_addr_t                   host;
        std::string                 server_name;
        std::map<int, std::string>  error_page;
        std::map<std::string, Location> locations;
        
    public:
        Server(void);
        ~Server(void);

        struct sockaddr_in address;

        int 	                    setPort(int port);
        int 	                    setFd(int fd);
        int 	                    setHost(std::string host);
        int 	                    setServerName(std::string server_name);
        int 	                    setClientMax(int client_max_body_size);
        void 	                    setMethod(std::string tmp);
        void 	                    setErrorPage(int key, std::string tmp);
        void 	                    setCgiEx(std::string tmp);
 
        int         				getPort(void) const;
        int         				getFd(void) const;
        int        					getClientMax(void) const;
        in_addr_t    				getHost(void) const;
        std::string 				getServerName(void) const;
        std::vector<std::string> 	getMethod(void) const;
        std::string 				getMethodi(int i) const;
        std::vector<std::string>    getCgiEx(void) const;
        std::string                 getCgiExi(int i) const;
        std::map<int, std::string>& getErrorPage(void);
        std::string                 getErrorPagei(int i);

		int setLoc(std::string key, Location data);
        Location                 getLoci(std::string str);
        std::map<std::string, Location>& getLoc(void);

};

#endif