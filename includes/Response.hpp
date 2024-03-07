#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream> // stringstream

#include "User.hpp"
// #include "Response.hpp"

class Response {

    public :
        Response();
        ~Response();
        Response(const Response& cpy);
        Response& operator=(const Response& rhs);

        void setMessages();
        void setBackupPages();

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

        void        processRequest();
        void        errorData(User user);
        std::string makeHeader();

    private :
        // Request request;

        std::map<int, std::string>  messages;
        std::map<int, std::string>  errorBackup;

        int                         _status;
		int                         _clength;
        std::string                 _filePath;
        std::string                 _data;
		std::string                 _ctype;
        std::stringstream           _body;
        std::stringstream           _content;
};

#endif