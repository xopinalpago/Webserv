#include "Request.hpp"

Request::Request(void)
{
	allRequest = "";
	method = "";
	uri = "";
	version = "";
	host = "";
	// contentType = "";
	contentLength = 0;
	keepAlive = 0;	
	return ;
}

Request::~Request(void)
{
	return ;
}

std::string		Request::getAllRequest(void)
{
	return (this->allRequest);
}

std::string		Request::getMethod(void)
{
	return (this->method);
}

std::string		Request::getUri(void)
{
	return (this->uri);
}

std::string		Request::getVersion(void)
{
	return (this->version);
}

std::string		Request::getHost(void)
{
	return (this->host);
}

// std::string		Request::getContentType(void)
// {
// 	return (this->contentType);
// }

// std::string		Request::getPathFile(void)
// {
// 	return (this->pathFile);
// }

int	Request::getContentLength(void)
{
	return (this->contentLength);
}

int	Request::getKeepAlive(void)
{
	return (this->keepAlive);
}


int	Request::setAllRequest(std::string request)
{
    if (request.length() == 0)
    {
        return (1);
    }
	this->allRequest.append(request);
    return (0);
}

// int	Request::setMethod(std::string method)
// {
//     if (method.length() == 0)
//     {
//         return (1);
//     }
// 	this->method = method;
//     return (0);
// }

// int	Request::setUri(std::string uri)
// {
//     if (uri.length() == 0)
//     {
//         return (1);
//     }
// 	this->uri = uri;
//     return (0);
// }

// int	Request::setVersion(std::string version)
// {
//     if (version.length() == 0)
//     {
//         return (1);
//     }
// 	this->version = version;
//     return (0);
// }

int	Request::setHost(std::string host)
{
    if (host.length() == 0)
    {
        return (1);
    }
	this->host = host;
    return (0);
}

// int		Request::setContentType(std::string cType)
// {
//     if (cType.length() == 0)
//     {
//         return (1);
//     }
// 	this->contentType = cType;
//     return (0);
// }

// int		Request::setPathFile(std::string pathFile)
// {
//     if (pathFile.length() == 0)
//     {
//         return (1);
//     }
// 	this->pathFile = pathFile;
//     return (0);
// }

int	Request::setContentLength(unsigned int cLength)
{
    if (cLength < 0)
    {
        return (1);
    }
	this->contentLength = cLength;
    return (0);
}

int	Request::setKeepAlive(int alive)
{
    if (alive != 0 && alive != 1)
    {
        return (1);
    }
	this->keepAlive = alive;
    return (0);
}

void Request::splitString(void)
{
    std::istringstream iss(this->allRequest);
    std::string line;

    while (std::getline(iss, line, '\n'))
    {
        this->vAllRequest.push_back(line);
    }
}

Server	Request::getServer(void) const
{
	return (this->server);
}

Location	Request::getLocation(void)
{
	return (this->loc);
}

void	Request::setServer(Server server)
{
	this->server = server;
	return ;
}

int	Request::parseRequest(void)
{
    splitString();
	int	fpos = vAllRequest[0].find(' ', 0);
	int	lpos = vAllRequest[0].find(' ', fpos + 1);
	method	= vAllRequest[0].substr(0, fpos);
	uri = vAllRequest[0].substr(fpos + 1, lpos - fpos - 1);
	version = vAllRequest[0].substr(lpos + 1, lpos - vAllRequest[0].size());

	if (method.length() == 0 || uri.length() == 0 || version.length() == 0)
		return (1);
	// std::cout << std::endl;
	// std::cout << "method= " << method << std::endl;
	// std::cout << "uri= " << uri << std::endl;
	// std::cout << "version= " << version << std::endl;
	// std::cout << std::endl;
	
	for (size_t i = 0; i < vAllRequest.size(); i++)
	{
		int pos = 0;
		if (vAllRequest[i].find("Host") == 0)
		{
			pos = vAllRequest[i].find(' ');
			host = vAllRequest[i].substr(pos + 1, vAllRequest[i].size() - pos);
		}
		else if (vAllRequest[i].find("Connection") == 0)
		{
			pos = vAllRequest[i].find(' ');
			std::string tmp = vAllRequest[i].substr(pos + 1, vAllRequest[i].size() - pos);
			if (tmp == "keep-alive")
			{
				keepAlive = 1;
			}
			else
			{
				keepAlive = 0;
			}
		}
		else if (vAllRequest[i].find("Referer") == 0)
		{
			pos = vAllRequest[i].find(' ');
			referer = vAllRequest[i].substr(pos + 1, vAllRequest[i].size() - pos);
		}
		else if (vAllRequest[i].find("Accept") == 0)
		{
			pos = vAllRequest[i].find(' ');
			accept = vAllRequest[i].substr(pos + 1, vAllRequest[i].size() - pos);
		}
	}
	contentLength = allRequest.size();
	return (0);
}

std::vector<std::string> splitBySlash(std::string str) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, '/')) {
        tokens.push_back(token);
    }
    return tokens;
}

int Request::setLocation(std::map<std::string, Location> locations)
{
	size_t len_corresp = 0;
	// std::string key = "/";
	std::map<std::string, Location>::iterator iter = locations.end();

	std::vector<std::string> uri = splitBySlash(this->getUri());

	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
		std::cout << "this->getUri() = " << this->getUri() << std::endl;
		std::cout << "it->first = " << it->first << std::endl;
		std::vector<std::string> tmp = splitBySlash(it->first);

		size_t minElement = (uri.size() < tmp.size()) ? uri.size() : tmp.size();
		for (size_t i = 1; i <= minElement; i++)
		{
			if (uri[i - 1] == tmp[i - 1] && i > len_corresp)
			{
				len_corresp = i;
				iter = it;
			}
		}
		
		// std::string tmp = "";
		// if (this->getUri() != "/" && this->getUri()[this->getUri().length() - 1] == '/')
		// 	tmp = this->getUri().substr(0, this->getUri().length() - 1);
		// else
		// 	tmp = this->getUri();
		
        // if(tmp.find(it->first) == 0)
        // {
        //        if(it->first == "/" || tmp.length() == it->first.length() || tmp[it->getPath().length()] == '/')
        //        {
        //             if(it->getPath().length() > biggest_match)
        //             {
        //                 biggest_match = it->getPath().length();
        //                 location_key = it->getPath();
        //             }
        //        }
        // }
		// if (uri == it->first)
		// {
		// 	this->loc = it->second;
		// 	return (0);
		// }
	}
	// if (len_corresp > 0)
	// 	return (0)
	this->loc = iter->second;
	return (0);
}