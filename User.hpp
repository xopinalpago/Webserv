#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <stdio.h>

class User
{
	private:

	public:
		int id;
		std::string request;
		void getRequest(void);
		std::string getPath(void);
		User(void);
		User(int id);
		~User(void);
};

#endif