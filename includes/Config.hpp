#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "Server.hpp"
#include "Utils.hpp"
#include "Launcher.hpp"

class Config {

    public :
        Config(void);
        ~Config(void);
		int         fillServer(Server &server);
        int         getLineFile(std::string &filename, Launcher &launcher);
        int         missElement(Server &server);
        // int         cleanMethod(int serverToRead, Server &server);
        // int         cleanCGI(int serverToRead, Server &server);
        int         cleanError(Server &server);
        // void        setNbConfig(std::string &filename, std::string to_find);
		// int         getNbConfig(void);
        int         makePort(Server &server, std::string str, int &nbPort);
        int         makeHost(Server &server, std::string str, int &nbHost);
        int         makeServerName(Server &server, std::string str, int &nbServerName);
        int         makeRoot(Location &loc, std::string str, int &nbRoot);
        int         makeIndex(Location &loc, std::string str, int &nbIndex);
        int         makeClientMax(Server &server, std::string str, int &nbClientMax);
        int         makeAutoIndex(Location &loc, std::string str, int &nbAutoIndex);
        std::string getValue(std::string line);
        int	        fillLocation(Server &server);
        int         checkLocation(Server &server);
        std::string getValueLoc(std::string line, int &pos);
        int makeMethod(Location &loc, std::string str, int &nbAllowMethods);
        int makeCgiEx(Location &loc, std::string str, int &nbCgiEx);
        int makeCgiPath(Location &loc, std::string str, int &nbCgiPath);
        int missElementLocRoot(Location &loc);

    private :
        // int nb_config;
        std::vector<std::string> serverConfig;
        // std::vector<std::string> method;
        std::vector<std::string> location;
        std::vector<std::string> error_page;

};

#endif