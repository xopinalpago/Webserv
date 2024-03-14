#include "Server.hpp"

Server::Server(void)
{
	this->fd = 0;
	this->port = 0;
	this->client_max_body_size = 0;
    this->host = 0;
	this->server_name = "";
    return ;
}

Server::~Server(void)
{
    return ;
}

int Server::setPort(int port)
{
    if (port > 65535 || port <= 0)
    {
        return (1);
    }
	this->port = port;
    return (0);
}

int Server::setFd(int fd)
{
	this->fd = fd;
    return (fd);
}

int Server::setHost(std::string host)
{
    if (host.length() == 0)
    {
        return (1);
    }
 	if (host == "localhost")
		host = "127.0.0.1";   
	this->host = inet_addr(host.data());
    return (0);
}

int Server::setServerName(std::string server_name)
{
    if (server_name.length() == 0)
    {
        return (1);
    }
	this->server_name = server_name;
    return (0);
}

// int Server::setIndex(std::string index)
// {
//     if (index.length() == 0)
//     {
//         return (1);
//     }
// 	this->index = index;
//     return (0);
// }

int Server::setClientMax(int client_max_body_size)
{
    if (client_max_body_size > 10000)
    {
        return (1);
    }
	this->client_max_body_size = client_max_body_size;
    return (0);
}

std::string Server::getRoot(void) const
{
    return (this->root);
}

int Server::setRoot(std::string root)
{
    if (root.length() == 0)
    {
        return (1);
    }
	this->root = root;
    return (0);
}

// int Server::setDirectory(std::string directory_listing)
// {
//     if (directory_listing.length() == 0)
//     {
//         return (1);
//     }
// 	this->directory_listing = directory_listing;
//     return (0);
// }

// void Server::setMethod(std::string tmp)
// {
//     if (Utils::inVector(this->method, tmp)) 
//     {
//         this->method.push_back(tmp);
//     }
//     return ;
// }

// std::vector<std::string> Server::getMethod(void) const
// {
//     return (this->method);
// }

// std::string Server::getMethodi(int i) const
// {
//     if (i < 0 || i >= (int)this->method.size())
//         return ("");
//     return (this->method[i]);
// }

// void Server::setCgiEx(std::string tmp)
// {
//     if (Utils::inVector(this->cgi_extension, tmp)) 
//     {
//         this->cgi_extension.push_back(tmp);
//     }
//     return ;
// }

// std::vector<std::string> Server::getCgiEx(void) const
// {
//     return (this->cgi_extension);
// }

// std::string Server::getCgiExi(int i) const
// {
//     if (i < 0 || i >= (int)this->cgi_extension.size())
//             return ("");
//     return (this->cgi_extension[i]);
// }

void Server::setErrorPage(int key, std::string tmp)
{
    this->error_page[key] = tmp;
    return ;
}

std::map<int, std::string>&  Server::getErrorPage(void)
{
    return (this->error_page);
}

std::string Server::getErrorPagei(int i)
{
    return (this->error_page[i]);
}

int Server::getPort(void) const
{
    return (this->port);
}

int Server::getFd(void) const
{
    return (this->fd);
}

in_addr_t Server::getHost(void) const
{
    return (this->host);
}

std::string Server::getServerName(void) const
{
    return (this->server_name);
}

// std::string Server::getIndex(void) const
// {
//     return (this->index);
// }

int Server::getClientMax(void) const
{
    return (this->client_max_body_size);
}

// std::string Server::getDirectory(void) const
// {
//     return (this->directory_listing);
// }

int Server::setLoc(std::string key, Location data)
{
    if (this->locations.find(key) == this->locations.end()) 
    {
        this->locations[key] = data;
        return (0);
    }
    return (1);
}

Location Server::getLoci(std::string key)
{
    return (this->locations[key]);
}		
        
std::map<std::string, Location>& Server::getLoc(void)
{
	return (this->locations);
}
