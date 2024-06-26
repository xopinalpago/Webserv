#include "Launcher.hpp"

Launcher::Launcher(void)
{
	totalBytes = 0;
	max_sd = 0;
	FD_ZERO(&writefds);
	FD_ZERO(&readfds);
	return ;
}

Launcher::~Launcher(void)
{
	return ;
}

Launcher::Launcher(const Launcher& cpy) {
    *this = cpy;
}

Launcher& Launcher::operator=(const Launcher& rhs) {
	if (this != &rhs) {
    	timeout = rhs.timeout;
    	readfds = rhs.readfds;
    	writefds = rhs.writefds;
    	tmp_readfds = rhs.tmp_readfds;
    	tmp_writefds = rhs.tmp_writefds;
    	rc = rhs.rc;
    	max_sd = rhs.max_sd;
		new_sd = rhs.new_sd;
		end_server = rhs.end_server;
		totalBytes = rhs.totalBytes;
		Users = rhs.Users;
		Servers = rhs.Servers;
	}
	return *this;
}

void 	Launcher::addServerOfClient(int listen_sock, User *client)
{
	for (std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
	{
		if (listen_sock == it->second.getFd())
		{
			client->AddServerPtr(it->second);
		}
	}
}

void Launcher::listenServer(Server &server)
{
	int addrlen = sizeof(server.address);
	User  new_client;

	new_sd = accept(server.getFd(), (struct sockaddr *)&server.address, (socklen_t*)&addrlen);
	if (new_sd < 0)
		throw LauncherException("accept failed");
	
	std::cout << "New incoming connection: " << new_sd << std::endl;
	new_client.setFd(new_sd);
	addServerOfClient(server.getFd(), &new_client);
	Users[new_sd] = new_client;
	FD_SET(new_sd, &readfds);
	if (new_sd > max_sd)
		max_sd = new_sd;
	return ;
}

void    Launcher::closeAllConnection(void)
{
    for (int i = 3; i <= max_sd; ++i)
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

int Launcher::readbuf(User &user, Request& request, char *bf) {

	memset(bf, 0, sizeof(*bf));
	rc = recv(user.getFd(), bf, BUFFER_SIZE, MSG_DONTWAIT);
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
	return rc;
}


std::string Launcher::extractBody(Request& request) {

	std::string body;
    std::string req = request.getAllRequest();
    size_t header_end = req.find("\r\n\r\n");
	header_end = req.find("\r\n\r\n");
	if (header_end != std::string::npos)
		body = req.substr(header_end + 4, req.size() - header_end - 4);
	else
		body = "";
	return body;
}

int Launcher::readServer(User &user)
{
    int rc = BUFFER_SIZE;
	char bf[BUFFER_SIZE + 1];
	Request request;

    while (rc == BUFFER_SIZE) {
		rc = readbuf(user, request, bf);
		if (rc <= 0)
			return rc;
	}
	// std::cout << "**************** REQUEST ****************" << std::endl;
	// std::cout << request.getAllRequest() << std::endl;
	// std::cout << "************************ ****************" << std::endl;

	request.setBody(extractBody(request));

	if (request.parseRequest(user.getServerVec())) {
		closeConnection(user.getFd());
		return (0);
	}
	
	while (request.getContentLength() > 0 && extractBody(request).length() < request.getContentLength() && request.getContentLength() < 10000) {
		rc = BUFFER_SIZE;
		while (rc == BUFFER_SIZE) {
			rc = readbuf(user, request, bf);
			if (rc <= 0)
				return rc;
		}
		request.setBody(extractBody(request));
	}

	user.setRequest(request);
	FD_CLR(user.getFd(), &readfds);
	FD_SET(user.getFd(), &writefds);

	std::cout << std::endl << LIGHT_RED;
	std::cout << "**************** REQUEST HEADER ****************" << std::endl;
	std::cout << request.getAllRequest().substr(0, request.getAllRequest().find('\n')) << std::endl;
	// std::cout << request.getAllRequest() << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << RESET << std::endl;

	return (1);
}

void	Launcher::sendServer(User &user)
{
	s_socketInfo info;
	info.max_sd = max_sd;
	info.readfds = readfds;
	info.writefds = writefds;

	Response *res = new Response(user.getRequest(), &info);

	int rc3 = send(user.getFd(), res->getFinalRes().c_str(), res->getFinalRes().size(), 0);
	if (rc3 < 0)
	{
		closeConnection(user.getFd());
		return ;
	}
	else if (rc3 == 0)
	{
		closeConnection(user.getFd());
		return ;
	}
	else
		user.updateTime();
	
	std::cout << std::endl << CYAN;
	std::cout << "**************** RESPONSE HEADER ****************" << std::endl;
	// std::cout << res->getFinalRes() << std::endl;
	std::cout << res->getFinalRes().substr(0, res->getFinalRes().find('\n')) << std::endl;
	std::cout << "*************************************************" << std::endl;
	std::cout << RESET << std::endl;
	
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
        throw LauncherException("socket failed");
	}
	if (server.getFd() > max_sd)
		max_sd = server.getFd();
	if ((rc = setsockopt(server.getFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		throw LauncherException("setsockopt failed");
	}
	std::memset(&server.address, 0, sizeof(server.address));
    server.address.sin_family = AF_INET;
	server.address.sin_addr.s_addr = server.getHost();
	server.address.sin_port = htons(server.getVecPorti(0));
	server.setPort(server.getVecPorti(0));
    std::cout << "Listening on port: " << server.getPort() << std::endl;
	if (bind(server.getFd(), (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
        throw LauncherException("bind failed");
    }
	Servers[server.getFd()] = server;
    return (0);
}

int Launcher::initServer(Server &server, int port)
{
    int on = 1;
	FD_ZERO(&writefds);
	if (server.setFd(socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		close(server.getFd());
        throw LauncherException("socket failed");
	}
	if (server.getFd() > max_sd)
		max_sd = server.getFd();
	if ((rc = setsockopt(server.getFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		throw LauncherException("setsockopt failed");
	}
	std::memset(&server.address, 0, sizeof(server.address));
    server.address.sin_family = AF_INET;
	server.address.sin_addr.s_addr = server.getHost();
	server.address.sin_port = htons(port);
	server.setPort(port);
	std::cout << "Listening on port: " << server.getPort() << std::endl;
    if (bind(server.getFd(), (struct sockaddr *)&server.address, sizeof(server.address)) < 0)
	{
        throw LauncherException("bind failed");
    }
	Servers[server.getFd()] = server;
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

	if (Servers.size() <= 0)
		throw LauncherException("No Server");
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
