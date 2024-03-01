#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <iostream>

#include "Pages.hpp"

class Cgi {

    public :
        Cgi(void);
        ~Cgi(void);

        std::string findParameters();

    private :
        Pages page;
};

#endif