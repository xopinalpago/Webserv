#ifndef PAGES_HPP
#define PAGES_HPP

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include "User.hpp"

class Pages
{
	private:

	public:
		int status;
		int clength;
		std::string message;
		std::string ctype;
		std::string displayPage(std::string file, std::string method);
		bool cgiExtension(std::string file_path, std::string extension);
		Pages(void);
		~Pages(void);
};

#endif