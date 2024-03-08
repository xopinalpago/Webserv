#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Cgi.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Utils.hpp"

class User
{
	private:
		Server server;
		Request request;
		int fd;
		// int status;
	
	public:
		User(void);
		User(Server &server);
		~User(void);

		int			getFd(void);
		Request 	getRequest(void);
		Server 		getServer(void);

		void 	setFd(int fd);
		int 	setServer(std::map<int, Server> Servers);
		void 	setRequest(Request request);

		bool scriptExt(std::string *file_path, std::string ext, std::string method);
};

#endif