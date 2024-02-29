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
	// printf("fpos = %d lpos = %d path_file = %s\n", fpos, lpos, path_file.c_str());

	// std::ifstream file("/index.html");
	if (!path_file.compare("/"))
		path_file = "/pages/index.html";
	else
		path_file = "/pages" + path_file;
	path_file = path_file.insert(0, ".");
    return (path_file);
}
