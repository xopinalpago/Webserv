#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class Utils {

    public :
        Utils(void);
        ~Utils(void);
		Utils(const Utils& cpy);
        Utils& operator=(const Utils& rhs);
		static int 			fileExists(std::string filename);
		static int 			stringToInt(std::string str);
		static int			inVector(std::vector<std::string> vec, std::string elem);
		static std::string 	trim(std::string& str);
		static std::string 	trimTab(std::string& str);
};

#endif