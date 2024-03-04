#include "Server.hpp"

int		g_exit = 0;

Server::Server(void)
{
    return ;
}

Server::~Server(void)
{
    return ;
}

void Server::errorFunction(std::string word)
{
	std::cerr << word << " failed" << std::endl;
}

void	handleSigint(int sig)
{
	if (sig == SIGINT)
	{
		g_exit = 1;
	}
}

int Server::initServer(void)
{
    int on = 1;

    if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
        return(errorFunction("socket"), 1);
    if ((rc = setsockopt(server_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0))
	{
		close(server_fd);
		return(errorFunction("setsockopt"), 1);
	}
    if ((rc = fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC)) < 0)
	{
        close(server_fd);
        return(errorFunction("fcntl"), 1);
	}
	std::memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
	memcpy(&address.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	address.sin6_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
        close(server_fd);
        return(errorFunction("bind"), 1);
    }
    if (listen(server_fd, 10) < 0)
	{
        close(server_fd);
        return(errorFunction("listen"), 1);
    }
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_SET(server_fd, &readfds);
	max_sd = server_fd;
	timeout.tv_sec  = 15 * 60;
	timeout.tv_usec = 0;
    return (0);
}

void Server::listenServer(void)
{
	int addrlen = sizeof(address);
	// printf("  Listening socket is readable\n");
	new_sd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_sd < 0)
	{
		strerror(errno);
		end_server = true;
		return ;
	}
	// printf("  New incoming connection - %d\n", new_sd);
	Users[new_sd] = User(new_sd);
	FD_SET(new_sd, &readfds);
	if (new_sd > max_sd)
		max_sd = new_sd;
	return ;
}

int recving(int currSd, std::string *req)
{
    char bf[BUFFER_SIZE + 1];
    int rc = recv(currSd, bf, BUFFER_SIZE, 0);
    if (rc <= 0)
    {
        if (rc == -1)
            strerror(errno);
        return rc;
    }
    bf[rc] = 0;
    std::string str(bf, bf + rc);
    (*req).append(str);
    return rc;
}

int Server::readServer(int i) {
	
    int bytes = 0, rc = BUFFER_SIZE;
    std::string request = "";
    while (rc == BUFFER_SIZE) {
        rc = recving(i, &request);
        if (rc <= 0) {
            if (rc == -1)
                std::cout << strerror(errno) << std::endl;
            return rc;
        }
        bytes += rc;
    }
    std::cout << "Total bytes = " << bytes << std::endl;
	// std::cout << "****************************" << std::endl;
	// std::cout << request; // << std::endl;
	// std::cout << "TAILLE : " << request.length() << std::endl; 
	// std::cout << "****************************" << std::endl;
	Users[i].request = request;
	FD_CLR(i, &readfds);
	FD_SET(i, &writefds);
	return (1);
}


// int Server::readServer(int i)
// {
// 	// printf("  Descriptor %d is readable\n", i);
// 	std::string request_data;
// 	int bytes;
// 	memset(buffer, 0, sizeof(buffer));
// 	while ((bytes = recv(i, buffer, BUFFER_SIZE, 0)) > 0) {
// 		buffer[bytes] = '\0';
// 		request_data.append(buffer, bytes);
// 		// Vérifier si nous avons atteint la fin de l'en-tête de la requête
//         size_t header_end = request_data.find("\r\n\r\n");
//         if (header_end != std::string::npos) {
// 			std::cout << "fin du header" << std::endl;
//             // Nous avons atteint la fin de l'en-tête de la requête
//             // Extraire le corps de la requête
// 			memset(buffer, 0, sizeof(buffer));
// 			bytes = recv(i, buffer, BUFFER_SIZE, 0);
// 			buffer[bytes] = '\0';
// 			std::cout << "bbuffer" << std::endl;
// 			std::cout << "|" << buffer << "|" << std::endl;
// 			// request_data.append(buffer, bytes);
// 			// header_end = request_data.find("\r\n\r\n");
//             // std::string request_body = request_data.substr(header_end + 4);

//             // Traiter le corps de la requête
//             // std::cout << "Corps de la requête :" << std::endl;
//             // std::cout << "****************************" << std::endl;
//             // std::cout << buffer << std::endl;
//             // std::cout << "****************************" << std::endl;

//             break;
// 		}
// 	}
// 	if (bytes == -1) {
// 		std::cout << strerror(errno) << std::endl;
// 		// strerror(errno);
// 	}
// 	std::cout << "Bytes recus = " << bytes << std::endl;
// 	// std::cout << "	Requete recue : " << request_data.size() << std::endl;
// 	// std::cout << "****************************" << std::endl;
// 	// std::cout << request_data << std::endl;
// 	// std::cout << "****************************" << std::endl;
// 	// std::cout << "	Buffer :" << std::endl;
// 	// std::cout << "****************************" << std::endl;
// 	// std::cout << buffer << std::endl;
// 	// std::cout << "****************************" << std::endl;

// 	Users[i].request = request_data;
// 	FD_CLR(i, &readfds);
// 	FD_SET(i, &writefds);
// 	return (1);
// }

// int Server::readServer(int i)
// {
// 	// printf("  Descriptor %d is readable\n", i);
// 	std::string all = "";
// 	int rc2 = BUFFER_SIZE;
// 	while (rc2 == BUFFER_SIZE)
// 	{
// 		char bf[BUFFER_SIZE + 1];
// 		rc2 = recv(i, buffer, BUFFER_SIZE, 0);
// 		if (rc2 <= 0)
// 		{
// 			if (rc2 == -1)
// 				strerror(errno);
// 			return (rc2);
// 		}
// 		bf[rc2] = 0;
// 		std::string str(bf, bf + rc2);
// 		all.append(str);
// 		len = rc2;		
// 		printf("  %d bytes received\n", len);
// 		rc2 = 1;
// 	}
// 	printf("****************************\n");
// 	std::cout << buffer << std::endl;
// 	std::cout << "TAILLE : " << strlen(buffer) << std::endl; 
// 	printf("****************************\n");
// 	Users[i].request = buffer;
// 	FD_CLR(i, &readfds);
// 	FD_SET(i, &writefds);
// 	return (1);
// }

void	Server::sendServer(int i)
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


int Server::runServer(void)
{
	end_server = false;
	signal(SIGINT, handleSigint); //?
    while (end_server == false)
	{
		// pourquoi des fd temporaires ?
		std::memcpy(&tmp_readfds, &readfds, sizeof(readfds));
		std::memcpy(&tmp_writefds, &writefds, sizeof(writefds));
		// std::cout << "Waiting for select..." << std::endl;
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
			if (FD_ISSET(i, &tmp_readfds))
			{
				if (i == server_fd)
				{
					listenServer();
				}
				else
				{
					int res = readServer(i);
					if (res == 0)
					{
						// printf("  Connection closeddddddddddddd\n");
					    close(i);
						FD_CLR(i, &readfds);
						FD_CLR(i, &writefds);
						if (i == max_sd)
						{
							while (FD_ISSET(max_sd, &readfds) == false)
								max_sd -= 1;
						}
					}
				}
			}
			if (FD_ISSET(i, &tmp_writefds))
			{
				sendServer(i);
			}
		}
		if (g_exit == 1)
		{
			// dprintf(2, "ddddddddddddddddddd\n");
			break ;
		}
    }
    for (int i=0; i <= max_sd; ++i)
    {
        if (FD_ISSET(i, &readfds))
        close(i);
    }
	return (0);
}