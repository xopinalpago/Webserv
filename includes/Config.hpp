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
        int GetLineFile(void);
        int	StringToInt(std::string str);
        int GetNbConfig(const std::string& filename, const std::string& to_find);
        std::string trim(const std::string& str);
        std::vector<std::string> serverConfig;
        std::vector<std::string> method;
        std::vector<std::string> cgi_extension;
        // Server server;
        int nb_config;
        int countOccurrences(const std::string& filename, const std::string& to_find);
    private :

};

#endif