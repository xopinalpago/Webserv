#include "User.hpp"

User::User(void)
{
    return ;
}

User::User(Server &server)
{
	this->server = server;
	return ;
}

User::~User(void)
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

int User::setServer(std::map<int, Server> Servers)
{
	std::string hostname;
	if (request.getHost().find(':') != std::string::npos)
		hostname = request.getHost().substr(0, request.getHost().length() - (request.getHost().length() - request.getHost().find(':')));
	else
		hostname = request.getHost();

    for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->second.getServerName() == hostname)
		{
			server = it->second;
			request.setServer(it->second);
			return (0);
		}
	}
	std::map<int, Server>::iterator itemp = Servers.begin();
	if (itemp != Servers.end())
	{
		server = itemp->second;
		request.setServer(itemp->second);
	}
	return (1);
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

Request 	User::getRequest(void) const
{
	return (this->request);
}

void User::setRequest(Request request)
{
	this->request = request;
	return ;
}

Response	User::getResponse(void)
{
	return this->response;
}
