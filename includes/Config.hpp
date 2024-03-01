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
		int ParseFile(void);
        int GetLineFile(void);
        int	StringToInt(std::string str);
        std::string trim(const std::string& str);
        std::vector<std::string> serverConfig;
        std::vector<std::string> method;
        std::vector<std::string> cgi_extension;
        Server server;
    private :

};

#endif