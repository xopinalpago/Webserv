#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>
#include <sstream>

#include "Server.hpp"

class Request
{
	private:
		std::string		allRequest;
		std::string		body;
		std::string		method;
		std::string		uri;
		std::string		version;
		std::string		host;
		std::string		contentType;
		std::string		contentId;
		std::string     referer;
		std::string     accept;
		// std::string		pathFile;
		unsigned int	contentLength;
		int				keepAlive;
		std::vector<std::string> vAllRequest;
		Server			server;
		Location		loc;

	public:
		Request();
		~Request();
		Request(const Request& cpy);
        Request& operator=(const Request& rhs);

		std::string		getAllRequest(void);
		std::string		getBody(void);
		std::string		getMethod(void);
		std::string		getUri(void);
		std::string		getVersion(void);
		std::string		getHost(void);
		std::string		getContentType(void);
		std::string		getContentId(void);
		// std::string		getPathFile(void);
		unsigned int	getContentLength(void);
		int				getKeepAlive(void);
		Server			getServer(void) const;
		Location		getLocation(void);

		int		setAllRequest(std::string request);
		int		setBody(std::string body);
		int		setLocation(std::map<std::string, Location> locations);
		// int		setMethod(std::string method);
		// int		setUri(std::string uri);
		// int		setVersion(std::string version);
		int		setHost(std::string host);
		// int		setContentType(std::string cType);
		// int		setPathFile(std::string str);
		unsigned int		setContentLength(unsigned int cLength);
		int		setKeepAlive(int alive);
		void	setServer(Server server);
		int		parseRequest(void);
		void 	splitString(void);


};



#endif
