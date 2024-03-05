#include "User.hpp"

User::User(void)
{
    return ;
}

User::User(int id)
{
	this->id = id;
    return ;
}

User::~User(void)
{
    return ;
}

void User::getRequest(void)
{
    return ;
}

std::string User::getPath(void)
{
    // printf("****************************\n");
	// printf("%s\n", request.c_str());
	// printf("****************************\n");
	int fpos = request.find(" ", 0);
	int lpos = request.find(" ", fpos + 1);
	std::string path_file = request.substr(fpos + 1, lpos - fpos - 1);
	if (!path_file.compare("/"))
		path_file = "/pages/index.html";
	else
		path_file = "/pages" + path_file;
	path_file = path_file.insert(0, ".");
    return (path_file);
}

std::string User::getMethod(void) {
	int lpos = request.find(" ", 0);
	std::string method = request.substr(0, lpos);
	return method;
}