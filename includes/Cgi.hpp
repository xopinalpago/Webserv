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
#include <stdio.h>
#include <fcntl.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

#include "Request.hpp"

typedef struct socketInfo {
	fd_set					readfds;
    fd_set					writefds;
	int						max_sd;
} s_socketInfo;

class Cgi {

    public :
        Cgi(void);
        Cgi(std::string filePath, s_socketInfo* infos);
        ~Cgi(void);
        Cgi(const Cgi& cpy);
        Cgi& operator=(const Cgi& rhs);

        int execCGI(Request request);
        int create_env(Request request);
        int mapToChar();
        void displayEnv();
        void freeEnv();
        void free_tabs();
        std::string decodeQuery(std::string query);
        std::string extractQuery(Request request);

        std::map<std::string, std::string> getEnv() const { return this->_env; }
        char **getCenv() const { return this->_cenv; }
        int getCgiFd() const { return this->_cgiFd; }
        void findArgs(Request& request);
        int execScript(int *fd_in, int *fd_out);
        int writePipe(int *fd_in, int *fd_out, std::string body);
        void    closeConnection(int i);
        void	closeAllConnection(void);

    private :
        std::map<std::string, std::string> _env;
        char **_cenv;
        int                 _cgiFd;
        std::string         _cgiFile;
        std::string         _filePath;
        char **_args;
        char *exec;
        s_socketInfo *_infos;
};

#endif