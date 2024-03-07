#include "User.hpp"

User::User(void)
{
    return ;
}

// User::User(int id)
// {
// 	this->id = id;
//     return ;
// }

User::User(Server &server)
{
	this->server = server;
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

Server User::getServer(void) const
{
	return (this->server);
}

void User::setFd(int fd)
{
	this->fd = fd;
	return ;
}

int User::getFd(void)
{
	return (this->fd);
}

bool User::scriptExt(std::string *file_path, std::string ext, std::string method) {

	if (method == "GET")
		*file_path = file_path->substr(0, file_path->find('?'));
	if (file_path->length() >= ext.length()) {
		return (file_path->compare(file_path->length() - ext.length() , ext.length(), ext) == 0);
	}
	return false;
}

std::string User::getPath(std::string method)
{
	int fpos = request.find(" ", 0);
	int lpos = request.find(" ", fpos + 1);
	std::string path_file = request.substr(fpos + 1, lpos - fpos - 1);
	if (!path_file.compare("/"))
		path_file = "pages/index.html";
	else if (scriptExt(&path_file, ".py", method) || scriptExt(&path_file, ".php", method) || method == "DELETE") // definir en fct du fichier de config
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