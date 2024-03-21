#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>
#include <vector>
#include <map>
#include <arpa/inet.h>

#include "Utils.hpp"

class Location
{
	private:
		std::string								path;
		std::string								root;
		std::vector<std::string>				index;
      	std::vector<std::string>    			method;
		std::vector<std::string>    			cgi_extension;
        std::map<std::string, std::string>	    cgi_path;
		std::string							    redirectionPath;
		std::string							    upload_dir;
		int									    redirectionCode;
		int										autoindex;
		int										client_max_body_size;

	public:
		Location();
		~Location();
		Location(const Location& cpy);
        Location& operator=(const Location& rhs);

		int setAutoindex(std::string autoindex);
		int setClientMax(int client_max_body_size);
		int setPath(std::string path);
		int setRoot(std::string root);
		void setMethod(std::string tmp);
		int setIndex(std::string index);
		void setCgiEx(std::string data);
		void setCgiPath(std::string key, std::string data);
		int setRedirectionPath(std::string data);
		void setRedirectionCode(int code);
		int setUploadDir(std::string path);

		std::string getPath(void) const;
		std::string getRoot(void) const;
		std::vector<std::string>	getIndex(void) const;
		std::string					getIndexi(int i) const;
		std::vector<std::string> 	getMethod(void) const;
        std::string 				getMethodi(int i) const;
		std::string 				getCgiExi(int i) const;
		std::vector<std::string> 	getCgiEx(void) const;
		std::string 				getCgiPathi(std::string key);
		std::map<std::string, std::string> 	getCgiPath(void) const;
		std::string 				getRedirectionPath(void) const;
		int 				getRedirectionCode(void) const;
		std::string 				getUploadDir(void) const;

		int getAutoindex(void) const;
};

#endif