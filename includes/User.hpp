#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

class User
{
	private:
		Server 		server;
		Request 	request;
		Response 	response;
		int 		fd;
		time_t      lastTime;
	
	public:
		User(void);
		User(Server &server);
		~User(void);
		User(const User& cpy);
        User& operator=(const User& rhs);
		
		Location		loc;

		int			getFd(void) const;
		Request 	getRequest(void) const;
		Server 		getServer(void) const;
		Response	getResponse(void) const;
		time_t     	getLastTime(void) const;

		void        updateTime();

		void 		setFd(int fd);
		int 		setServer(std::map<int, Server> Servers);
		void 		setRequest(Request request);
};

#endif