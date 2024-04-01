#include "Request.hpp"

Request::Request(void)
{
	allRequest = "";
	method = "";
	uri = "";
	version = "";
	host = "";
	contentType = "";
	contentLength = 0;
	keepAlive = 0;	
	return ;
}

Request::~Request(void)
{
	return ;
}

Request::Request(const Request& cpy) {
    *this = cpy;
}

Request& Request::operator=(const Request& rhs) {
    if (this != &rhs) {
        allRequest = rhs.allRequest;
        body = rhs.body;
        method = rhs.method;
        uri = rhs.uri;
        version = rhs.version;
        host = rhs.host;
        contentType = rhs.contentType;
        contentId = rhs.contentId;
        referer = rhs.referer;
        accept = rhs.accept;
        contentLength = rhs.contentLength;
		keepAlive = rhs.keepAlive;
		vAllRequest = rhs.vAllRequest;
		server = rhs.server;
		loc = rhs.loc;
    }
    return *this;
}

std::string		Request::getAllRequest(void)
{
	return (this->allRequest);
}

std::string		Request::getBody(void)
{
	return (this->body);
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

std::string		Request::getContentType(void)
{
	return (this->contentType);
}

std::string		Request::getContentId(void)
{
	return (this->contentId);
}

unsigned int	Request::getContentLength(void)
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

int Request::setBody(std::string body)
{
	if (body.length() == 0)
	{
		return (1);
	}
	this->body = body;
	return (0);
}

int	Request::setHost(std::string host)
{
    if (host.length() == 0)
    {
        return (1);
    }
	this->host = host;
    return (0);
}

unsigned int	Request::setContentLength(unsigned int cLength)
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


int Request::setServer(std::vector<Server> servers)
{
	if (servers.size() == 1)
	{
		this->server = servers[0]; 
		this->setLocation(this->getServer().getLoc());
		return (0);
	}
	
	std::string hostname;
	if (this->getHost().find(':') != std::string::npos) {
		hostname = this->getHost().substr(0, this->getHost().length() - (this->getHost().length() - this->getHost().find(':')));
	}
	else {
		hostname = this->getHost().substr(0, this->getHost().size() - 1);
	}

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		if (it->getServerName() == hostname)
		{
			server = *it;
			this->setLocation(this->getServer().getLoc());
			return (0);
		}
	}
	server = servers[0];
	this->setLocation(this->getServer().getLoc());
	return (0);
}


int	Request::parseRequest(std::vector<Server> servers)
{
    splitString();
	int	fpos = vAllRequest[0].find(' ', 0);
	int	lpos = vAllRequest[0].find(' ', fpos + 1);
	method	= vAllRequest[0].substr(0, fpos);
	uri = vAllRequest[0].substr(fpos + 1, lpos - fpos - 1);
	version = vAllRequest[0].substr(lpos + 1, vAllRequest[0].size() - lpos - 2);

	if (method.length() == 0 || uri.length() == 0 || version.length() == 0)
		return (1);
	
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
		else if (vAllRequest[i].find("Content-Length:") == 0)
		{
			size_t start = vAllRequest[i].find(':') + 2;
			std::istringstream iss(vAllRequest[i].substr(start));
			iss >> contentLength;
		}
		else if (this->getMethod() == "POST" && vAllRequest[i].find("Content-Type") == 0 && contentType.size() == 0)
		{
			pos = vAllRequest[i].find(' ');
			contentType = vAllRequest[i].substr(pos + 1, vAllRequest[i].find(';') - pos - 1);
			if (contentType == "multipart/form-data") {
				pos = vAllRequest[i].find("boundary=");
				contentId = vAllRequest[i].substr(pos + 9, vAllRequest[i].size() - (pos + 9) - 1);
			}
		}
	}
	setServer(servers);
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
	std::string locPath;
	Location tempLoc;
	size_t	end;
	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		locPath = it->first;
		if (tempLoc.getPath().length() == 0 && locPath == "/")
		{
			tempLoc = it->second;
			continue;
		}
		for (end = 0; uri[end] != '\0' && locPath[end] != '\0' && uri[end] == locPath[end]; end++)
		{
			;
		}
		if (locPath[end] == '\0' && (uri[end] == '\0' || uri[end] == '/'))
		{
			if (tempLoc.getPath().length() == 0 || (tempLoc.getPath().length() != 0 && tempLoc.getPath().length() < it->first.length()))
				tempLoc = it->second;
		}
	}
	if (tempLoc.getPath().length() != 0)
	{
		this->loc = tempLoc;
	}
	else
		return (1);
	return (0);
}
