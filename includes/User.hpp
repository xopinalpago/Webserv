#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

#include "Cgi.hpp"
#include "Server.hpp"

class User
{
	private:
		Server server;
		int fd;
	public:
		std::string request;
		void setFd(int fd);
		int	getFd(void);
		void getRequest(void);
		std::string getPath(void);
		std::string getMethod(void);
		Server getServer(void);
		User(void);
		User(int id);
		User(Server &server);
		~User(void);
};

#endif