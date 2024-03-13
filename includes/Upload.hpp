#ifndef UPLOAD_HPP
# define UPLOAD_HPP

#include <cstdio>
#include <cstdlib>

#include "Request.hpp"

class Upload {

    public :
        Upload();
        ~Upload();
        Upload(Request request);
        Upload(const Upload& cpy);
        Upload& operator=(const Upload& rhs);

        int doUpload();
        void parseUpload();

    private :
        Request _request;
        Server  _server;
        std::string filename;
        std::string fileBody;

};

#endif