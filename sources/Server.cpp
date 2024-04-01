#include "Server.hpp"

Server::Server(void)
{
	this->fd = 0;
	this->client_max_body_size = 0;
    this->host = 0;
	this->server_name = "";
    return ;
}

Server::~Server(void)
{
    return ;
}

Server::Server(const Server& cpy) {
    *this = cpy;
}

Server& Server::operator=(const Server& rhs) {
	if (this != &rhs) {
    	fd = rhs.fd;
        vecPort = rhs.vecPort;
    	port = rhs.port;
    	client_max_body_size = rhs.client_max_body_size;
        host = rhs.host;
        str_host = rhs.str_host;
        server_name = rhs.server_name;
        root = rhs.root;
        index = rhs.index;
        error_page = rhs.error_page;
        locations = rhs.locations;
        address = rhs.address;
	}
	return *this;
}

int Server::setVecPort(int port)
{
    if (port > 65535 || port <= 0)
    {
        return (1);
    }
	this->vecPort.push_back(port);
    return (0);
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
    if (this->host == INADDR_NONE) {
        return (1);
    }
    return (0);
}

int Server::setStrHost(std::string str_host)
{
    if (str_host.length() == 0)
    {
        return (1);
    }
	this->str_host = str_host;
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

int Server::setIndex(std::string index)
{
    if (index.length() == 0)
    {
        return (1);
    }
	this->index.push_back(index);
    return (0);
}

unsigned int Server::setClientMax(unsigned int client_max_body_size)
{
    if (client_max_body_size > 1000000)
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

std::vector<int> Server::getVecPort(void) const
{
    return (this->vecPort);
}

int Server::getPort(void) const
{
    return (this->port);
}

int Server::getVecPorti(int i) 
{
    if (i >= 0 && i < (int)this->vecPort.size())
    {
        return (this->vecPort[i]);
    }
    return (0);
}

int Server::getFd(void) const
{
    return (this->fd);
}

in_addr_t Server::getHost(void) const
{
    return (this->host);
}

std::string Server::getStrHost(void) const
{
    return (this->str_host);
}

std::string Server::getServerName(void) const
{
    return (this->server_name);
}

std::vector<std::string> Server::getIndex(void) const
{
    return (this->index);
}

std::string Server::getIndexi(int i) const
{
    if (i < 0 || i >= (int)this->index.size())
        return ("");
    return (this->index[i]);
}

unsigned int Server::getClientMax(void) const
{
    return (this->client_max_body_size);
}

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
