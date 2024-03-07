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

        int execCGI(User user);
        int create_env(User user);
        int mapToChar();
        void displayEnv();
        void freeEnv();
        std::string decodeQuery(std::string query);
        std::string extractQuery(User user);
        bool scriptExt(std::string ext, std::string method);

        bool IsCgiExtension(std::string file, User user);
        bool authorizedMethod(User user);

        std::string displayPage(std::string method, User &user);
        void setMessages();
        void setBackupPages();
        void errorData(User user);
        std::string makeHeader();

        /* getters */
        int getStatus() const { return _status; }
        std::string getFilePath() const { return _filePath; }
        std::string getData() const { return _data; }
        int getClength() const { return _clength; }
        std::string getCtype() const { return _ctype; }
        // std::stringstream getBody() { return body; }
        // std::stringstream getContent() { return content; }

        std::map<std::string, std::string> getEnv() const {
            return this->_env;
        }
        char **getCenv() const {
            return this->_cenv;
        }

        /* setters */
        void setStatus( int status ) { _status = status; }
        void setFilePath( std::string filePath ) { _filePath = filePath; }
        void setData( std::string data ) { _data = data; };
        void setClength( int clength ) { _clength = clength; }
        void setCtype( std::string ctype ) { _ctype = ctype; }
        // void setBody();
        // void setContent();


    private :
        std::map<std::string, std::string> _env;
        char **_cenv;
        std::map<int, std::string> messages;
        std::map<int, std::string> errorBackup;

        int                 _status;
        int                 _cgiFd;
        std::string         _cgiFile;
        std::string         _filePath;
        std::string         _data;
        std::stringstream   _body;
        std::stringstream   _content;
		int                 _clength;
		std::string         _ctype;
        std::string setPathFile(std::string path_file, User &user);

};

#endif