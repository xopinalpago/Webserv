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

#include "Pages.hpp"

class Pages;

class Cgi {

    public :
        Cgi(void);
        ~Cgi(void);

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

    private :
        // Pages page;
        std::map<std::string, std::string> _env;
        char **_cenv;

};

#endif