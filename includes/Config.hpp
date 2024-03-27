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
	private :
        std::vector<std::string> serverConfig;
        std::vector<std::string> location;
        std::vector<std::string> error_page;

		int         fillServer(Server &server);
        int         missElement(Server &server);
        int         cleanError(Server &server);
        int         makePort(Server &server, std::string str);
        int         makeHost(Server &server, std::string str, int &nbHost);
        int         makeServerName(Server &server, std::string str, int &nbServerName);
        int         makeRoot(Location &loc, std::string str, int &nbRoot);
        int         makeRoot(Server &server, std::string str, int &nbRoot);
        int         makeIndex(Location &loc, std::string str, int &nbIndex);
        int         makeIndex(Server &server, std::string str, int &nbIndex);
        int         makeUploadDir(Location &loc, std::string str, int &nbUploadDir);
        int         makeClientMax(Server &server, std::string str, int &nbClientMax);
        int         makeAutoIndex(Location &loc, std::string str, int &nbAutoIndex);
        int         makeRedirection(Location &loc, std::string str, int &nbRedirection, Server &server);
        int	        fillLocation(Server &server);
        int         makeMethod(Location &loc, std::string str, int &nbAllowMethods);
        int         missElementListFiles(Location &loc);
        int         makeCgiEx(Location &loc, std::string str, int &nbCgiEx);
        int         makeCgiPath(Location &loc, std::string str, int &nbCgiPath);
        int         missElementLoc(Location &loc, Server &server);
        int         missElementCgi(Location &loc);
        int         setDefaultMethods(Location &loc);
        std::string getValueLoc(std::string line, int &pos);
        std::string getValue(std::string line);

    public :
        Config(void);
        ~Config(void);
        Config(const Config& cpy);
        Config& operator=(const Config& rhs);

        int         getLineFile(std::string &filename, Launcher &launcher);

		class ConfigException : public std::exception {
		public :
			ConfigException(std::string err) throw() {
				_err = "Config Error: " + err;
			}
			virtual const char* what() const throw() {
				return (_err.c_str());
			}
			~ConfigException() throw() {}
		
		private:
			std::string _err;
		};
};

#endif