#ifndef PAGES_HPP
#define PAGES_HPP

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string>

#include "User.hpp"
#include "Cgi.hpp"

class Cgi;
class User;

class Pages
{
	private:

	public:
		Pages(void);
		~Pages(void);
		int status;
		int clength;
		std::string message;
		std::string ctype;
		// mettre en prive et ajouter des getters
		
		std::string displayPage(std::string file, std::string method, std::map<int, User> Users, int i);
		static bool cgiExtension(std::string file_path, std::string extension);
};

#endif