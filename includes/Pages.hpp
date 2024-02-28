#ifndef PAGES_HPP
#define PAGES_HPP

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

class Pages
{
	private:

	public:
		int status;
		int clength;
		std::string message;
		std::string ctype;
		std::string displayPage(std::string file);
		Pages(void);
		~Pages(void);
};

#endif