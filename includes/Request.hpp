#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>
#include <sstream>

#include "Server.hpp"

class Request
{
	private:
		std::string		method;
		std::string		uri;
		std::string		version;
		std::string		host;
		// std::string		contentType;
		std::string     referer;
		std::string     accept;
		// std::string		pathFile;
		int				contentLength;
		int				keepAlive;
		std::vector<std::string> vAllRequest;
		Server			server;

	public:
		Request();
		~Request();

		std::string		allRequest;
		std::string		getAllRequest(void);
		std::string		getMethod(void);
		std::string		getUri(void);
		std::string		getVersion(void);
		std::string		getHost(void);
		// std::string		getContentType(void);
		// std::string		getPathFile(void);
		int	getContentLength(void);
		int				getKeepAlive(void);
		Server			getServer(void) const;

		int		setAllRequest(std::string request);
		int		setMethod(std::string method);
		int		setUri(std::string uri);
		int		setVersion(std::string version);
		int		setHost(std::string host);
		// int		setContentType(std::string cType);
		// int		setPathFile(std::string str);
		int		setContentLength(unsigned int cLength);
		int		setKeepAlive(int alive);
		void	setServer(Server server);
		int		parseRequest(void);
		void 	splitString(void);

};



#endif
