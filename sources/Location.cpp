#include "Location.hpp"

Location::Location(void)
{
    return ;
}

Location::~Location(void)
{
    return ;
}

int Location::setAutoindex(std::string autoindex)
{
    if (autoindex == "on")
        this->autoindex = 1;
    else if (autoindex == "off")
        this->autoindex = 0;
    else
    {
        return (1);
    }
    return (0);
}

int Location::setClientMax(int client_max_body_size)
{
    if (client_max_body_size > 10000)
    {
        return (1);
    }
	this->client_max_body_size = client_max_body_size;
    return (0);
}

int Location::setPath(std::string path)
{
    if (path.length() == 0)
    {
        return (1);
    }
	this->path = path;
    return (0);
}

int Location::setRoot(std::string root)
{
    if (root.length() == 0)
    {
        return (1);
    }
	this->root = root;
    return (0);
}

void Location::setMethod(std::string tmp)
{
    if (Utils::inVector(this->method, tmp)) 
    {
        this->method.push_back(tmp);
    }
    return ;
}

void Location::setCgiEx(std::string tmp)
{
    if (Utils::inVector(this->cgi_extension, tmp)) 
    {
        this->cgi_extension.push_back(tmp);
    }
    return ;
}

std::string Location::getRoot(void) const
{
    return (this->root);
}

int Location::setIndex(std::string index)
{
    if (index.length() == 0)
    {
        return (1);
    }
	this->index = index;
    return (0);
}

std::vector<std::string> Location::getMethod(void) const
{
    return (this->method);
}

std::string Location::getMethodi(int i) const
{
    if (i < 0 || i >= (int)this->method.size())
        return ("");
    return (this->method[i]);
}

void Location::setCgiPath(std::string key, std::string data)
{
    if (this->cgi_path.find(key) == this->cgi_path.end()) 
    {
        this->cgi_path[key] = data;
    }
    return ;
}

std::vector<std::string> Location::getCgiEx(void) const
{
    return (this->cgi_extension);
}

std::string Location::getCgiExi(int i) const
{
    if (i < 0 || i >= (int)this->cgi_extension.size())
        return ("");
    return (this->cgi_extension[i]);
}

std::string Location::getPath(void) const
{
	return (this->path);
}

std::string Location::getCgiPathi(std::string key)
{
    if (this->cgi_path.find(key) != this->cgi_path.end()) 
    {
        return (this->cgi_path[key]);
    }
    return ("");
}		
        
std::map<std::string, std::string> Location::getCgiPath(void) const
{
	return (this->cgi_path);
}