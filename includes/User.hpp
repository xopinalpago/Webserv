#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Cgi.hpp"
#include "Config.hpp"
#include "Launcher.hpp"
#include "Server.hpp"
#include "Utils.hpp"

class Cgi;
class Config;
class Launcher;
class Server;
class Utils;

class User
{
	private:

	public:
		int id;
		std::string request;
		void getRequest(void);
		std::string getPath(void);
		std::string getMethod(void);
		User(void);
		User(int id);
		~User(void);
};

#endif