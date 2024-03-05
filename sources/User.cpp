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
	Server server;
	int fpos = request.find(" ", 0);
	int lpos = request.find(" ", fpos + 1);
	std::string path_file = request.substr(fpos + 1, lpos - fpos - 1);
	if (!path_file.compare("/"))
		path_file = "pages/index.html";
	else if (Cgi::cgiExtension(path_file, server, ".py") || Cgi::cgiExtension(path_file, server, ".php")) // definir en fct du fichier de config
		return path_file.substr(1, path_file.length() - 1);
	else
		path_file = "pages" + path_file;
    return (path_file);
}

std::string User::getMethod(void) {
	int lpos = request.find(" ", 0);
	std::string method = request.substr(0, lpos);
	return method;
}