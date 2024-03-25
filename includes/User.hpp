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
		Request 	request;
		Response 	response;
		int 		fd;
		time_t      lastTime;
		std::vector< Server> 	ServerVec;
	
	public:
		User(void);
		User(Server &server);
		~User(void);
		User(const User& cpy);
        User& operator=(const User& rhs);
		
		Location		loc;

		int			getFd(void) const;
		Request 	getRequest(void) const;
		Response	getResponse(void) const;
		time_t     	getLastTime(void) const;
		std::vector< Server> 		getServerVec(void) const;
		void		AddServerPtr(Server newServerVec);
		void        updateTime();

		void 		setFd(int fd);
		void 		setRequest(Request request);
};

#endif