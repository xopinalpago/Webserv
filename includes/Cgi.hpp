#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <iostream>
#include <map>
#include <algorithm>
# include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdio>
#include <sstream>
#include <fstream>

#include "User.hpp"

class User;

class Cgi {

    public :
        Cgi(void);
        ~Cgi(void);

        // mettre en prive et dans une class Response
        int status;
		int clength;
		std::string message;
		std::string ctype;

        int execCGI(std::string file_path);
        void create_env();
        int mapToChar();
        void displayEnv();
        void freeEnv();
        std::map<std::string, std::string> getEnv() const {
            return this->_env;
        }
        char **getCenv() const {
            return this->_cenv;
        }
        static bool cgiExtension(std::string file_path, std::string extension);
        std::string displayPage(std::string method, User &user);

    private :
        std::map<std::string, std::string> _env;
        char **_cenv;

};

#endif