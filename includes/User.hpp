#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Cgi.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Utils.hpp"
#include "Response.hpp"

class User
{
	private:
		Server server;
		Request request;
		Response response;
		int fd;
		// int status;
	
	public:
		User(void);
		User(Server &server);
		~User(void);

		int			getFd(void);
		Request 	getRequest(void) const;
		Server 		getServer(void) const;
		Response	getResponse(void);

		void 	setFd(int fd);
		int 	setServer(std::map<int, Server> Servers);
		void 	setRequest(Request request);

};

#endif