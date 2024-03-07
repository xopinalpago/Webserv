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

Server User::getServer(void)
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

Request 	User::getRequest(void)
{
	return (this->request);
}

void User::setRequest(Request request)
{
	this->request = request;
	return ;
}
