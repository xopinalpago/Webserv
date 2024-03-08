#include "Launcher.hpp"

Launcher::Launcher(void)
{
	return ;
}

Launcher::~Launcher(void)
{
	return ;
}

void Launcher::errorFunction(std::string word)
{
	std::cerr << word << " failed" << std::endl;
}

int Launcher::initConfig(std::string &filename)
{
	Config	config;

	config.setNbConfig(filename, "server ");
	if (config.getNbConfig() == 0)
		return (1);
	if (config.GetLineFile(filename))
		return (1); 
	for (int i = 1; i <= config.getNbConfig(); i++)
	{
		Server server;
		if (config.ParseFile(i, server))
			return (1);
		if (config.MissElement(server))
			return (1);
		if (initServer(server))
			return (1);
	}
	if (config.getNbConfig() > 1)
		if (checkServers())
			return (1);
	return (0);
}

void Launcher::listenServer(Server &server)
{
	int addrlen = sizeof(server.address);
	User  new_client;

	new_sd = accept(server.getFd(), (struct sockaddr *)&server.address, (socklen_t*)&addrlen);
	if (new_sd < 0)
	{
		strerror(errno);
		end_server = true;
		return ;
	}
	new_client.setFd(new_sd);
	Users[new_sd] = new_client;
	FD_SET(new_sd, &readfds);
	if (new_sd > max_sd)
		max_sd = new_sd;
	return ;
}

void    Launcher::closeConnection(int fd)
{
    if (FD_ISSET(fd, &writefds))
        FD_CLR(fd, &writefds);
    if (FD_ISSET(fd, &readfds))
        FD_CLR(fd, &readfds);
    if (fd == max_sd)
        max_sd--;
    close(fd);
    Users.erase(fd);
}

int Launcher::readServer(User &user)
{
	int bytes = 0;
    int rc = BUFFER_SIZE;
	char bf[BUFFER_SIZE + 1];
    // std::string request = "";

	Request request;

    while (rc == BUFFER_SIZE) {
		memset(bf, 0, sizeof(bf));
		rc = recv(user.getFd(), bf, BUFFER_SIZE, 0);
		if (rc == 0)
		{
			closeConnection(user.getFd());
			return (rc);
		}
		else if (rc < 0)
		{
			closeConnection(user.getFd());
			return (rc);
		}
        // if (rc <= 0)
		// {
        //     if (rc == -1)
        //         std::cout << strerror(errno) << std::endl;
        //     return rc;
        // }
		bf[rc] = 0;
		// request.append(bf, rc);
		request.allRequest.append(bf, rc);
        bytes += rc;
    }

	request.parseRequest();
	user.setRequest(request);
	user.setServer(Servers);
	FD_CLR(user.getFd(), &readfds);
	FD_SET(user.getFd(), &writefds);
	return (1);
}

void	Launcher::sendServer(User &user)
{
	Response *res = new Response(user.getRequest());

	int rc3 = send(user.getFd(), res->getFinalRes().c_str(), res->getFinalRes().size(), 0);
	if (rc3 < 0)
		strerror(errno); // gestion d'erreur ?
	FD_CLR(user.getFd(), &writefds);
	FD_SET(user.getFd(), &readfds);

	delete res;
}

void Launcher::checkServerName(void)
{
	for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
    {
		for (std::map<int, Server>::iterator it2 = Servers.begin(); it2 != it; ++it2)
        {
            if (it2->second.getHost() == it->second.getHost() && it2->second.getPort() == it->second.getPort())
            {
				it->second.setFd(it2->second.getFd());
            }
        }
    }
}

int Launcher::initServer(Server &server)
{
    int on = 1;
	FD_ZERO(&writefds);
	int fd_temp = 0;
    if ((fd_temp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return(errorFunction("socket"), 1);
    server.setFd(fd_temp);
	if ((rc = setsockopt(server.getFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		close(server.getFd());
		return(errorFunction("setsockopt"), 1);
	}
	std::memset(&server.address, 0, sizeof(server.address));
    server.address.sin_family = AF_INET;
	server.address.sin_addr.s_addr = server.getHost();
	server.address.sin_port = htons(server.getPort());
    if (bind(server.getFd(), (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
        close(server.getFd());
        return(errorFunction("bind"), 1);
    }
	Servers[server.getFd()] = server;
	max_sd = server.getFd();
    return (0);
}

int	Launcher::initSets(void)
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
    for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
    {
		if (listen(it->second.getFd(), 10) < 0)
		{
			close(it->second.getFd());
			return(errorFunction("listen"), 1);
		}
		if ((rc = fcntl(it->second.getFd(), F_SETFL, O_NONBLOCK, FD_CLOEXEC)) < 0)
		{
		    close(it->second.getFd());
		    return(errorFunction("fcntl"), 1);
		}
		FD_SET(it->second.getFd(), &readfds);
	}
	return (0);
}

int Launcher::runServer(void)
{
	end_server = false;

	checkServerName();
	if (initSets())
		return (1);
    while (end_server == false)
	{
		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;
		std::memcpy(&tmp_readfds, &readfds, sizeof(readfds));
		std::memcpy(&tmp_writefds, &writefds, sizeof(writefds));
		rc = select(max_sd + 1, &tmp_readfds, &tmp_writefds, NULL, &timeout);
		if (rc < 0)
		{
            std::cerr << "select() failed" << std::endl;
			return (1);
		}
		for (int i = 0; i <= max_sd; i++)
		{
			if (FD_ISSET(i, &tmp_readfds) && Servers.count(i))
			{
				listenServer(Servers.find(i)->second);
			}
			else if (FD_ISSET(i, &tmp_readfds) && Users.count(i))
			{
				readServer(Users.find(i)->second);
			}
			else if (FD_ISSET(i, &tmp_writefds) && Users.count(i))
			{
				sendServer(Users.find(i)->second);
			}
		}
    }
    for (int i=0; i <= max_sd; ++i)
    {
        if (FD_ISSET(i, &readfds))
        close(i);
    }
	return (0);
}

int Launcher::checkServers(void)
{
    std::vector<int> keys;

    for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
    {
        keys.push_back(it->first);
    }

    for (size_t i = 0; i < keys.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < keys.size(); ++j)
        {
            int key1 = keys[i];
            int key2 = keys[j];

            if (Servers[key1].getPort() == Servers[key2].getPort() &&
                Servers[key1].getHost() == Servers[key2].getHost() &&
                Servers[key1].getServerName() == Servers[key2].getServerName())
            {
                return (1);
            }
        }
    }
    return (0);
}
