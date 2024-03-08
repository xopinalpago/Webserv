#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream> // stringstream

#include "Cgi.hpp"

class Response {

    public :
        Response();
        Response(Request request);
        ~Response();
        Response(const Response& cpy);
        Response& operator=(const Response& rhs);

        void setMessages();
        void setBackupPages();
        void setTypes();

        /* setters */
        void setStatus( int status ) { _status = status; }
        void setFilePath( std::string filePath ) { _filePath = filePath; }
        void setData( std::string data ) { _data = data; };
        void setClength( int clength ) { _clength = clength; }
        void setCtype( std::string ctype ) { _ctype = ctype; }

        /* getters */
        int         getStatus() const { return _status; }
        int         getClength() const { return _clength; }
        std::string getFilePath() const { return _filePath; }
        std::string getData() const { return _data; }
        std::string getCtype() const { return _ctype; }
        std::string getFinalRes() const { return _finalRes; }

        void        processRequest();
        void        errorData();
        void        setPathFile();
        bool        IsCgiExtension(std::string file);
        std::string makeHeader();
        bool        authorizedMethod();

    private :
        Request                     _request;
        Server                      _server;

        std::map<int, std::string>          messages;
        std::map<int, std::string>          errorBackup;
        std::map<std::string, std::string>  types;

        int                         _status;
		int                         _clength;
        std::string                 _filePath;
        std::string                 _data;
		std::string                 _ctype;
        std::stringstream           _body;
        std::stringstream           _content;

        std::string                 _finalRes;
};

#endif