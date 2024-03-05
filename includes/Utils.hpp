#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <fstream>

class Utils {

    public :
        Utils(void);
        ~Utils(void);
		static int 			fileExists(std::string filename);
		static int 			stringToInt(std::string str);
		static std::string 	trim(std::string& str);
};

#endif