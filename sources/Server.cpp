#include "Server.hpp"

Server::Server(void)
{
    return ;
}

Server::~Server(void)
{
    return ;
}

int Server::setPort(int port)
{
    if (port > 65535)
    {
        return (1);
    }
	this->port = port;
    return (0);
}

void Server::setFd(int fd)
{
	this->fd = fd;
    return ;
}

int Server::setHost(std::string host)
{
    if (host.length() == 0)
    {
        return (1);
    }
	this->host = host;
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

int Server::setRoot(std::string root)
{
    if (root.length() == 0)
    {
        return (1);
    }
	this->root = root;
    return (0);
}

int Server::setIndex(std::string index)
{
    if (index.length() == 0)
    {
        return (1);
    }
	this->index = index;
    return (0);
}

// int Server::setErrorPage(std::string error_page)
// {
//     if (error_page.length() == 0)
//     {
//         return (1);
//     }
// 	this->error_page = error_page;
//     return (0);
// }

int Server::setClientMax(std::string client_max_body_size)
{
    if (client_max_body_size.length() == 0)
    {
        return (1);
    }
	this->client_max_body_size = client_max_body_size;
    return (0);
}

int Server::setDirectory(std::string directory_listing)
{
    if (directory_listing.length() == 0)
    {
        return (1);
    }
	this->directory_listing = directory_listing;
    return (0);
}

int Server::getPort(void)
{
    return (this->port);
}

int Server::getFd(void)
{
    return (this->fd);
}

std::string Server::getHost(void)
{
    return (this->host);
}

std::string Server::getServerName(void)
{
    return (this->server_name);
}

std::string Server::getRoot(void)
{
    return (this->root);
}

std::string Server::getIndex(void)
{
    return (this->index);
}

// std::string Server::getErrorPage(void)
// {
//     return (this->error_page);
// }

std::string Server::getClientMax(void)
{
    return (this->client_max_body_size);
}

std::string Server::getDirectory(void)
{
    return (this->directory_listing);
}
