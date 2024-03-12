#include "User.hpp"

User::User(void)
{
	lastTime = time(NULL);
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
	std::string hostname = "";
	int port = 0;
	if (request.getHost().find(':') != std::string::npos)
	{
		hostname = request.getHost().substr(0, request.getHost().length() - (request.getHost().length() - request.getHost().find(':')));
		port = Utils::stringToInt(request.getHost().substr(request.getHost().find(':') + 1, request.getHost().length() - request.getHost().find(':') - 2));
	}
	// else
	// 	hostname = request.getHost();

    for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (it->second.getServerName() == hostname && it->second.getPort() == port)
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
	request.setLocation(request.getServer().getLoc());
	return (1);
}

int User::getFd(void) const
{
	return (this->fd);
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

Response	User::getResponse(void) const
{
	return this->response;
}

time_t     User::getLastTime() const
{
    return (lastTime);
}

void	User::updateTime()
{
    lastTime = time(NULL);
}
