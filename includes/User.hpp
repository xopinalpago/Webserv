#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Cgi.hpp"
#include "Server.hpp"
#include "Request.hpp"

class User
{
	private:
		Server server;
		Request request;
		int fd;
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
};

#endif