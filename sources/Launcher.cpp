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

	config.nb_config = config.GetNbConfig(filename, "server ");
	if (config.nb_config == 0)
		return (1);
	if (config.GetLineFile(filename))
		return (1); 
	for (int i = 1; i <= config.nb_config; i++)
	{
		Server server;
		if (config.ParseFile(i, server))
			return (1);
		if (config.MissElement(server))
			return (1);
		std::cout << server.getPort() << std::endl;
		if (initServer(server))
			return (1);
		std::cout << "test" << std::endl;
	}
	return (0);
}

void Launcher::listenServer(Server &server)
{
	int addrlen = sizeof(address);
	printf("  Listening socket is readable\n");
	new_sd = accept(server.getFd(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_sd < 0)
	{
		std::cout << "config.nb_config" << std::endl;
		strerror(errno);
		end_server = true;
		return ;
	}
	printf("  New incoming connection - %d\n", new_sd);
	Users[new_sd] = User(new_sd);
	FD_SET(new_sd, &readfds);
	if (new_sd > max_sd)
		max_sd = new_sd;
	return ;
}

int Launcher::readServer(int i)
{
	printf("  Descriptor %d is readable\n", i);
	std::string all = "";
	int rc2 = BUFFER_SIZE;
	while (rc2 == BUFFER_SIZE)
	{
		char bf[BUFFER_SIZE + 1];
		rc2 = recv(i, buffer, BUFFER_SIZE, 0);
		if (rc2 <= 0)
		{
			if (rc2 == -1)
				strerror(errno);
			return (rc2);
		}
		bf[rc2] = 0;
		std::string str(bf, bf + rc2);
		all.append(str);
		len = rc2;		
		printf("  %d bytes received\n", len);
		rc2 = 1;
	}
	printf("****************************\n");
	std::cout << buffer << std::endl;
	// printf("%s\n", buffer);
	std::cout << "TAILLE : " << strlen(buffer) << std::endl; 
	printf("****************************\n");
	Users[i].request = buffer;
	FD_CLR(i, &readfds);
	FD_SET(i, &writefds);
	return (1);
}

void	Launcher::sendServer(int i)
{
	Pages page;
	std::string method = Users[i].getMethod();
	std::string content = page.displayPage(Users[i].getPath().c_str(), method);
	int rc3 = send(i, content.c_str(), content.size(), 0);
	if (rc3 < 0)
		strerror(errno);
	// std::cout << "******* content dans sendServer *******" << std::endl;
	// std::cout << content << std::endl;
	// std::cout << "***************************************" << std::endl;
	FD_CLR(i, &writefds);
	FD_SET(i, &readfds);	
}

int Launcher::initServer(Server &server)
{
    int on = 1;
	FD_ZERO(&writefds);
	int fd_temp = 0;
    if ((fd_temp = socket(AF_INET6, SOCK_STREAM, 0)) < 0) //pk AF_INET6 pour IPV6 et pas IPV4 ?
        return(errorFunction("socket"), 1);
    server.setFd(fd_temp);
	if ((rc = setsockopt(server.getFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		close(server.getFd());
		return(errorFunction("setsockopt"), 1);
	}
	std::memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
	memcpy(&address.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    // le serveur doit pouvoir ecouter plusieurs ports mais ici on en defini qu'un ?
	address.sin6_port = htons(server.getPort());
    if (bind(server.getFd(), (struct sockaddr *)&address, sizeof(address)) < 0)
	{
        close(server.getFd());
        return(errorFunction("bind"), 1);
    }
	std::cout << server.getFd() << std::endl;
	// Servers.insert(std::make_pair(server.getFd(), server));
	Servers[server.getFd()] = server;
	// pourquoi le plus grand fd est celui du socket ?
	max_sd = server.getFd();
	timeout.tv_sec  = 15 * 60;
	timeout.tv_usec = 0;
    return (0);
}

int	Launcher::initSets(void)
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
    for(std::map<int, Server>::iterator it = Servers.begin(); it != Servers.end(); ++it)
    {
		if (listen(it->first, 10) < 0)
		{
			close(it->first);
			return(errorFunction("listen"), 1);
		}
		if ((rc = fcntl(it->first, F_SETFL, O_NONBLOCK, FD_CLOEXEC)) < 0)
		{
		    close(it->first);
		    return(errorFunction("fcntl"), 1);
		}
		FD_SET(it->first, &readfds);
	}
	return (0);
}

int Launcher::runServer(void)
{
	end_server = false;

	if (initSets())
		return (1);
    while (end_server == false)
	{
		// pourquoi des fd temporaires ?
		std::memcpy(&tmp_readfds, &readfds, sizeof(readfds));
		std::memcpy(&tmp_writefds, &writefds, sizeof(writefds));
		std::cout << "Waiting for select..." << std::endl;
		// signifcation rc ?
		rc = select(max_sd + 1, &tmp_readfds, &tmp_writefds, NULL, &timeout);
		if (rc < 0)
		{
            std::cerr << "select() failed" << std::endl;
			return (1);
		}
		if (rc == 0)
		{
            std::cerr << "select() timed out" << std::endl;
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
				readServer(i);
			}
			else if (FD_ISSET(i, &tmp_writefds) && Users.count(i))
			{
				sendServer(i);
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
