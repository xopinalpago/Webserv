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
		int ParseFile(int serverToRead, Server &server);
        int GetLineFile(std::string &filename);
        int MissElement(Server &server);
        int cleanMethod(int serverToRead, Server &server);
        int cleanCGI(int serverToRead, Server &server);
        int cleanError(int serverToRead, Server &server);
        void setNbConfig(std::string &filename, std::string to_find);
		int getNbConfig(void);
    private :
        int nb_config;
        std::vector<std::string> serverConfig;
        std::vector<std::string> method;
        std::vector<std::string> cgi_extension;
        std::vector<std::string> error_page;

};

#endif