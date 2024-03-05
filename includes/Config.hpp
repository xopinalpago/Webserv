#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Server.hpp"

class Config {

    public :
        Config(void);
        ~Config(void);
		int ParseFile(int serverToRead, Server &server);
        int	StringToInt(std::string str);
        int GetLineFile(std::string &filename);
        int GetNbConfig(std::string &filename, std::string to_find);
        int MissElement(Server &server);
        int cleanMethod(int serverToRead, Server &server);
        int cleanError(int serverToRead, Server &server);
        std::string trim(std::string& str);
        std::vector<std::string> serverConfig;
        std::vector<std::string> method;
        std::vector<std::string> cgi_extension;
        std::vector<std::string> error_page;
        int nb_config;
    private :

};

#endif