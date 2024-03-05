#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <fstream>

#include "Cgi.hpp"
#include "Config.hpp"
#include "Launcher.hpp"
#include "Server.hpp"
#include "User.hpp"

class Cgi;
class Config;
class Launcher;
class Server;
class User;

class Utils {

    public :
        Utils(void);
        ~Utils(void);
		static int fileExists(std::string filename);
		static int stringToInt(std::string str);
};

#endif