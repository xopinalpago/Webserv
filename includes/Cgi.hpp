#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <sys/stat.h> // chmod

#include "Request.hpp"

class Cgi {

    public :
        Cgi(void);
        Cgi(std::string filePath);
        ~Cgi(void);

        int execCGI(Request request);
        int create_env(Request request);
        int mapToChar();
        void displayEnv();
        void freeEnv();
        std::string decodeQuery(std::string query);
        std::string extractQuery(Request request);

        std::map<std::string, std::string> getEnv() const { return this->_env; }
        char **getCenv() const { return this->_cenv; }
        int getCgiFd() const { return this->_cgiFd; }

    private :
        std::map<std::string, std::string> _env;
        char **_cenv;
        int                 _cgiFd;
        std::string         _cgiFile;
        std::string         _filePath;
};

#endif