#include "Launcher.hpp"

Launcher::Launcher(void)
{
	FD_ZERO(&writefds);
	FD_ZERO(&readfds);
	return ;
}

Launcher::~Launcher(void)
{
	return ;
}

// int Launcher::initConfig(std::string &filename)
// {
// 	Config	config;

// 	// config.setNbConfig(filename, "server ");
// 	// if (config.getNbConfig() == 0)
// 	// 	return (1);
// 	if (config.getLineFile(filename))
// 		return (1); 
// 	// for (int i = 1; i <= config.getNbConfig(); i++)
// 	// {
// 		// Server server;
// 		if (config.parseFile(i, server))
// 			return (1);
// 		if (config.missElement(server))
// 			return (1);
// 		if (initServer(server))
// 			return (1);
// 	}
// 	if (config.getNbConfig() > 1)
// 		if (checkServers())
// 			return (1);
// 	return (0);
// }

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
	
	std::cout << "New incoming connection: " << new_sd << std::endl;
	new_client.setFd(new_sd);
	Users[new_sd] = new_client;
	FD_SET(new_sd, &readfds);
	if (new_sd > max_sd)
		max_sd = new_sd;
	return ;
}

void    Launcher::closeAllConnection(void)
{
    for (int i = 0; i <= max_sd; ++i)
    {
		closeConnection(i);
    }
}

void    Launcher::closeConnection(int fd)
{
	if (FD_ISSET(fd, &writefds))
		FD_CLR(fd, &writefds);
	if (FD_ISSET(fd, &readfds))
		FD_CLR(fd, &readfds);
    if (fd == max_sd)
	{
        max_sd--;
	}
	std::cout << "Connection: " << fd << " closed..." << std::endl;
    close(fd);
	if (Users.find(fd) != Users.end())
		Users.erase(fd);
	if (Servers.find(fd) != Servers.end())
		Servers.erase(fd);
}

// void    Launcher::closeAllConnectionExcep(void)
// {
//     for (int fd = 0; fd <= max_sd; ++fd)
//     {
// 		if (FD_ISSET(fd, &writefds))
// 			FD_CLR(fd, &writefds);
// 		if (fd == max_sd)
// 		{
// 			max_sd--;
// 		}
// 		std::cout << "Connection: " << fd << " closed..." << std::endl;
// 		close(fd);
// 		if (Users.find(fd) != Users.end())
// 			Users.erase(fd);
// 		if (Servers.find(fd) != Servers.end())
// 			Servers.erase(fd);
//     }
// }

int Launcher::readServer(User &user)
{
    int rc = BUFFER_SIZE;
	char bf[BUFFER_SIZE + 1];
	Request request;
	totalBytes = 0;

	// premiere lecture
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
		else
			user.updateTime();
		bf[rc] = 0;
		totalBytes += rc;
		request.setAllRequest(bf);
	}
	// calculer le body
	std::string body;
    std::string req = request.getAllRequest();
    size_t header_end = req.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        body = req.substr(header_end + 4, req.size() - header_end - 4);
		request.setBody(body);
	} else {
        body = "";
		request.setBody(body);
	}
	// std::cout << "body size : " << body.size() << std::endl;

	// std::cout << "**************BODY***************" << std::endl;
	// std::cout << body << std::endl;
	// std::cout << "************************************" << std::endl;
	// parsing
	if (request.parseRequest())
	{
		closeConnection(user.getFd());
		return (0);
	}
	// si y'a un content 
	if (request.getContentLength() > 0 && body.length() < request.getContentLength()) {
		// std::cout << "body a lire" << std::endl;
		rc = BUFFER_SIZE;
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
			else
				user.updateTime();
			bf[rc] = 0;
			totalBytes += rc;
			request.setAllRequest(bf);
		}
		// recalculer le body :
		req = request.getAllRequest();
		header_end = req.find("\r\n\r\n");
		if (header_end != std::string::npos) {
			body = req.substr(header_end + 4, req.size() - header_end - 4);
			request.setBody(body);
		}
		else {
			body = "";
			request.setBody(body);
		}
		// std::cout << "body size2 : " << body.size() << std::endl;
	}

	user.setRequest(request);
	user.setServer(Servers);

	FD_CLR(user.getFd(), &readfds);
	FD_SET(user.getFd(), &writefds);

	// std::cout << "SIZE = " << request.getAllRequest().size() << std::endl;
	// std::cout << "URI = " << request.getUri() << std::endl;
	// std::cout << "URI = " << request.getUri() << std::endl;
	// std::cout << "**************REQUEST***************" << std::endl;
	// std::cout << request.getAllRequest() << std::endl;
	// std::cout << "************************************" << std::endl;

	return (1);
}

void	Launcher::sendServer(User &user)
{
	Response *res = new Response(user.getRequest());

	int rc3 = send(user.getFd(), res->getFinalRes().c_str(), res->getFinalRes().size(), 0);
	if (rc3 < 0)
		throw LauncherException("send failed");
	else
		user.updateTime();
	// std::cout << "******* content dans sendServer *******" << std::endl;
	// std::cout << res->getFinalRes().c_str() << std::endl;
	// std::cout << "***************************************" << std::endl;

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
	if (server.setFd(socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		close(server.getFd());
        throw LauncherInitException("socket failed");
	}
	if ((rc = setsockopt(server.getFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		close(server.getFd());
		throw LauncherInitException("setsockopt failed");
	}
	std::memset(&server.address, 0, sizeof(server.address));
    server.address.sin_family = AF_INET;
	server.address.sin_addr.s_addr = server.getHost();
	server.address.sin_port = htons(server.getPort());
    if (bind(server.getFd(), (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
        close(server.getFd());
        throw LauncherInitException("bind failed");
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
			throw LauncherException("listen failed");
		if ((rc = fcntl(it->second.getFd(), F_SETFL, O_NONBLOCK, FD_CLOEXEC)) < 0)
			throw LauncherException("fcntl failed");
		FD_SET(it->second.getFd(), &readfds);
	}
	return (0);
}

void	Launcher::checkTimeout(void)
{
    for(std::map<int, User>::iterator it = Users.begin() ; it != Users.end(); ++it)
    {
        if (time(NULL) - it->second.getLastTime() > 60)
        {
            closeConnection(it->first);
            return ;
        }
    }
}

int Launcher::runServer(void)
{
	end_server = false;

	checkServers();
	checkServerName();
	initSets();
    while (end_server == false)
	{
		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;
		std::memcpy(&tmp_readfds, &readfds, sizeof(readfds));
		std::memcpy(&tmp_writefds, &writefds, sizeof(writefds));
		rc = select(max_sd + 1, &tmp_readfds, &tmp_writefds, NULL, &timeout);
		if (rc < 0)
		{
			throw LauncherException("select failed");
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
		checkTimeout();
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
		int root = 0;
    	for (std::map<std::string, Location>::iterator itloc = it->second.getLoc().begin(); itloc != it->second.getLoc().end(); ++itloc)
		{
			if (itloc->first == "/")
				root = 1;
		}
		if (root == 0)
			throw LauncherException("No Root Location");
        keys.push_back(it->first);
    }
	if (Servers.size() == 1)
		return (0);
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
				throw LauncherException("Duplicates Server");
            }
        }
    }
    return (0);
}
