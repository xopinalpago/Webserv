#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "Server.hpp"
#include "Utils.hpp"

class Config {

    public :
        Config(void);
        ~Config(void);
		int         parseFile(int serverToRead, Server &server);
        int         getLineFile(std::string &filename);
        int         missElement(Server &server);
        int         cleanMethod(int serverToRead, Server &server);
        int         cleanCGI(int serverToRead, Server &server);
        int         cleanError(int serverToRead, Server &server);
        void        setNbConfig(std::string &filename, std::string to_find);
		int         getNbConfig(void);
        int         makePort(Server &server, std::string str, int &nbPort);
        int         makeHost(Server &server, std::string str, int &nbHost);
        int         makeServerName(Server &server, std::string str, int &nbServerName);
        int         makeRoot(Server &server, std::string str, int &nbRoot);
        int         makeIndex(Server &server, std::string str, int &nbIndex);
        int         makeClientMax(Server &server, std::string str, int &nbClientMax);
        int         makeDirectory(Server &server, std::string str, int &nbDirectory);
        std::string getValue(std::string line);

    private :
        int nb_config;
        std::vector<std::string> serverConfig;
        std::vector<std::string> method;
        std::vector<std::string> cgi_extension;
        std::vector<std::string> error_page;

};

#endif