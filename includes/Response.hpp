#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream> // stringstream
#include <dirent.h>

#include "Cgi.hpp"
#include "Upload.hpp"

class Response {

    public :
        Response();
        Response(Request request, s_socketInfo* infos);
        // Response(Request request, Launcher* launch);
        ~Response();
        Response(const Response& cpy);
        Response& operator=(const Response& rhs);

        void setMessages();
        void setBackupPages();
        void setTypes();

        /* setters */
        void setStatus( int status ) { _status = status; }
        void setFilePath( std::string filePath ) { _filePath = filePath; }
        void setClength( int clength ) { _clength = clength; }
        void setCtype( std::string ctype ) { _ctype = ctype; }

        /* getters */
        int         getStatus() const { return _status; }
        int         getClength() const { return _clength; }
        std::string getFilePath() const { return _filePath; }
        std::string getCtype() const { return _ctype; }
        std::string getFinalRes() const { return _finalRes; }

        void        processRequest();
        void        errorData();
        void        setPathFile();
        bool        IsCgiExtension(std::string file);
        std::string makeHeader();
        bool        authorizedMethod();
        bool        isDirectory(std::string path);
        int         directoryListing(const std::string& directoryPath);
        std::string getListJson();

    private :
        Request                     _request;
        Server                      _server;

        std::map<int, std::string>          messages;
        std::map<int, std::string>          backup;
        std::map<std::string, std::string>  types;

        int                         _status;
		int                         _clength;
        std::string                 _filePath;

        // std::string                 _uriPathInfo;
        // std::string                 _uriQuery;
        // std::string                 _scriptName;
        // std::string                 _uriPath;

		std::string                 _ctype;
        std::stringstream           _body;
        std::stringstream           _content;

        std::string                 _finalRes;
        s_socketInfo*               _infos;
};

#endif