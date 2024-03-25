#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

Config::Config(const Config& cpy) {
    *this = cpy;
}

Config& Config::operator=(const Config& rhs) {
	if (this != &rhs) {
    	serverConfig = rhs.serverConfig;
    	location = rhs.location;
    	error_page = rhs.error_page;
	}
	return *this;
}

int Config::getLineFile(std::string &filename, Launcher &launcher)
{
    std::ifstream infile(filename.c_str());
	int nbErrorPage = 0;
	

    if (infile.is_open())
	{
        std::string line;
        bool insideLocation = false;
		bool insideError = false; 
        while (std::getline(infile, line))
		{
            line = Utils::trim(line);
			line = Utils::trimTab(line);
            if (!line.empty())
			{
				if (line.find("server {") == 0)
					nbErrorPage = 0;
				if (line.find("location") == 0)
                    insideLocation = true;
				else if (line == "}" && insideLocation)
				{
					location.push_back(line);
                    insideLocation = false;
				}
				else if (line.find("error_page") == 0)
				{
					nbErrorPage++;
					if (nbErrorPage > 1)
						return (1);
					insideError = true;
				}
				else if (line == "}" && insideError)
                    insideError = false;
				else if (!insideLocation && !insideError && line.find("}") == 0)
				{
					Server server;
					fillServer(server);
					missElement(server);
					launcher.initServer(server);
					for (size_t i = 1; i < server.getVecPort().size(); i++)
					{
						Server serv = server;
						launcher.initServer(serv, server.getVecPorti(i));
					}
					serverConfig.clear();
					location.clear();
					error_page.clear();
				}
				if (!insideLocation && !insideError) 
                    serverConfig.push_back(line);
				else if (insideLocation)
					location.push_back(line);
				else if (insideError)
					error_page.push_back(line);
            }
        }
        infile.close();
    } 
	else
	{
        throw ConfigException("No Such File");
    }
    return (0);
}

//A REVOIR
int Config::cleanError(Server &server)
{
    for (size_t i = 0; i < error_page.size(); i++)
	{
		if (error_page[i].length() > 0)
		{
			if (error_page[i].find("error_page {"))
			{
				if (error_page[i][error_page[i].length() - 1] != ';')
					throw ConfigException("Invalid Error Page");
				if (!std::isdigit(error_page[i][0]))
					throw ConfigException("No Digit Number in Error Page");
				int first_sp = error_page[i].find(' ');
				if (first_sp < 0)
					throw ConfigException("No Space in Error Page");
				int error_num = Utils::stringToInt(error_page[i].substr(0, first_sp));
				//TESTER SI ERROR EST DANS LES ERREURS QU ON GERE
				std::string tmp_trim = Utils::trim(error_page[i]);
				std::string error_file = tmp_trim.substr(first_sp + 1, tmp_trim.size() - first_sp - 2);
				if (Utils::fileExists(error_file))
					throw ConfigException("No Corresponding File in Error Page");
				server.setErrorPage(error_num, error_file); 
			}
		}
	}
	return (0);
}

std::string Config::getValue(std::string line)
{
	int first_sp = line.find(' ');
	if (first_sp < 0)
		return ("");
	return(line.substr(first_sp + 1, line.size() - 2 - first_sp));
}

int Config::missElement(Server &server)
{
	if (server.getVecPort().size() <= 0)
		throw ConfigException("No Port");
	for (size_t i = 0; i < server.getVecPort().size(); i++)	{
		for (size_t j = i + 1; j < server.getVecPort().size(); j++) {
			if (server.getVecPorti(i) == server.getVecPorti(j))
				throw ConfigException("Duplicates Port");
		}
	}
	
	if (server.getClientMax() <= 0)
		throw ConfigException("No Client Max Body Size");
	if (server.getHost() <= 0)
		throw ConfigException("No Host");
	return (0);
}

int Config::makePort(Server &server, std::string str)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Port");
	std::string port_str = getValue(str);
	if (port_str.empty())
		throw ConfigException("Invalid Port");
	int port = Utils::stringToInt(port_str);
	if (port <= 0)
		throw ConfigException("Invalid Port");
	if (server.setVecPort(port))
		throw ConfigException("Invalid Port");
	return (0);
}

int Config::makeHost(Server &server, std::string str, int &nbHost)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Host");
	std::string host = getValue(str);
	if (host.length() == 0)
		throw ConfigException("Invalid Host");
	if (server.setHost(host))
		throw ConfigException("Invalid Host");
	server.setStrHost(host);
	nbHost++;
	if (nbHost > 1)
		throw ConfigException("Too many Host");
	return (0);
}

int Config::makeServerName(Server &server, std::string str, int &nbServerName)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Server Name");
	std::string server_name = getValue(str);
	if (server_name.length() == 0)
		throw ConfigException("Invalid Server Name");
	if (server.setServerName(server_name))
		throw ConfigException("Invalid Server Name");
	nbServerName++;
	if (nbServerName > 1)
		throw ConfigException("Too many Server Name");
	return (0);
}

int Config::makeRoot(Location &loc, std::string str, int &nbRoot)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Root");
	std::string root = getValue(str);
	if (root.length() == 0)
		throw ConfigException("Invalid Root");
	if (root.length() > 1 && root[0] == '/')
		root = root.substr(1);
	std::cout << "root = " << root << std::endl;
	if (loc.setRoot(root))
		throw ConfigException("Invalid Root");
	nbRoot++;
	if (nbRoot > 1)
		throw ConfigException("Too many Root");
	return (0);
}

// int Config::makeIndex(Location &loc, std::string str, int &nbIndex)
// {
// 	if (str[str.length() - 1] != ';')
// 		throw ConfigException("Invalid Index");
// 	std::string index = getValue(str);
// 	if (index.length() == 0)
// 		throw ConfigException("Invalid Index");
// 	if (loc.setIndex(index))
// 		throw ConfigException("Invalid Index");
// 	nbIndex++;
// 	if (nbIndex > 1)
// 		throw ConfigException("Too many Index");
// 	return (0);
// }

int Config::makeIndex(Location &loc, std::string str, int &nbIndex)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Index");
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string index = getValueLoc(str, pos);
		if (index.length() == 0)
			throw ConfigException("Invalid Index");
		loc.setIndex(index);
	}
	nbIndex++;
	if (nbIndex > 1)
		throw ConfigException("Too many Index");
	return (0);
}

int Config::makeUploadDir(Location &loc, std::string str, int &nbUploadDir)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Upload Dir");
	std::string uploadDir = getValue(str);
	if (uploadDir.length() == 0)
		throw ConfigException("Invalid Upload Dir");
	if (loc.setUploadDir(uploadDir))
		throw ConfigException("Invalid Upload Dir");
	nbUploadDir++;
	if (nbUploadDir > 1)
		throw ConfigException("Too many Upload Dir");
	return (0);
}

int Config::makeClientMax(Server &server, std::string str, int &nbClientMax)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Client Max Body Size");
	std::string client_max_body_size_str = getValue(str);
	if (client_max_body_size_str.empty())
		throw ConfigException("Invalid Client Max Body Size");
	unsigned int client_max_body_size = Utils::stringToInt(client_max_body_size_str);
	if (client_max_body_size <= 0)
		throw ConfigException("Invalid Client Max Body Size");
	if (server.setClientMax(client_max_body_size))
		throw ConfigException("Invalid Client Max Body Size");
	nbClientMax++;
	if (nbClientMax > 1)
		throw ConfigException("Too many Client Max Body Size");
	return (0);
}

int Config::makeRoot(Server &server, std::string str, int &nbRoot)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Root");
	std::string root = getValue(str);
	if (root.length() == 0)
		throw ConfigException("Invalid Root");
	if (server.setRoot(root))
		throw ConfigException("Invalid Root");
	nbRoot++;
	if (nbRoot > 1)
		throw ConfigException("Too many Root");
	return (0);
}

int Config::makeIndex(Server &server, std::string str, int &nbIndex)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Index");
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string index = getValueLoc(str, pos);
		if (index.length() == 0)
			throw ConfigException("Invalid Index");
		server.setIndex(index);
	}
	nbIndex++;
	if (nbIndex > 1)
		throw ConfigException("Too many Index");
	return (0);
}

int Config::makeAutoIndex(Location &loc, std::string str, int &nbAutoIndex)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Auto Index");
	std::string autoIndex = getValue(str);
	if (autoIndex.length() == 0)
		throw ConfigException("Invalid Auto Index");
	if (loc.setAutoindex(autoIndex))
		throw ConfigException("Invalid Auto Index");
	nbAutoIndex++;
	if (nbAutoIndex > 1)
		throw ConfigException("Too many Auto Index");
	return (0);
}

int Config::makeMethod(Location &loc, std::string str, int &nbAllowMethods)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Method");
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string method = getValueLoc(str, pos);
		if (method.length() == 0)
			throw ConfigException("Invalid Method");
		if (method == "GET" || method == "POST" || method == "DELETE")
			loc.setMethod(method);
		else if (method == "HEAD" || method == "PUT" || method == "CONNECT" || method == "OPTIONS" || method == "TRACE" || method == "PATCH")
			throw ConfigException("Method Not Implemented");
		else 
			throw ConfigException("Invalid Method");
	}
	nbAllowMethods++;
	if (nbAllowMethods > 1)
		throw ConfigException("Too many Method");
	return (0);
}

int Config::makeCgiEx(Location &loc, std::string str, int &nbCgiEx)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Cgi");
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string cigEx = getValueLoc(str, pos);
		if (cigEx.length() == 0)
			throw ConfigException("Invalid Cgi");
		loc.setCgiEx(cigEx);
	}
	nbCgiEx++;
	if (nbCgiEx > 1)
		throw ConfigException("Too many Cgi");
	return (0);
}

int Config::makeCgiPath(Location &loc, std::string str, int &nbCgiPath)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Cgi Path");
	int pos = 0;
	if (str.find(' ', pos) == str.npos)
		throw ConfigException("Invalid Cgi Path");
	while (str.find(' ', pos) != str.npos)
	{
		int	fpos = str.find(' ', pos);
		int	lpos = str.find(' ', fpos + 1);
		int	llpos = str.find(' ', lpos + 1);
		std::string key = str.substr(fpos + 1, lpos - fpos - 1);
		std::string path = str.substr(lpos + 1, llpos - lpos - 1);
		if (!path.empty() && path[path.length() - 1] == ';')
			path.erase(path.length() - 1);
		if (key.empty() || path.empty() || lpos < 0)
			throw ConfigException("Invalid Cgi Path");
		if (Utils::fileExists(path))
			throw ConfigException("Invalid Cgi Path");
		loc.setCgiPath(key, path);
		pos = llpos;
	}
	nbCgiPath++;
	if (nbCgiPath > 1)
		throw ConfigException("Too many Cgi Path");
	return (0);
}

int Config::makeRedirection(Location &loc, std::string str, int &nbRedirection, Server &server)
{
	if (str[str.length() - 1] != ';')
		throw ConfigException("Invalid Redirection");
	if (str.find(' ', 0) == str.npos)
		throw ConfigException("Invalid Redirection");
	int	fpos = str.find(' ', 0);
	int	lpos = str.find(' ', fpos + 1);
	int	llpos = str.find(' ', lpos + 1);
	int key = Utils::stringToInt(str.substr(fpos + 1, lpos - fpos - 1));
	std::string path = str.substr(lpos + 1, llpos - lpos - 1);
	if (!path.empty() && path[path.length() - 1] == ';')
		path.erase(path.length() - 1);
	if (path.empty() || lpos < 0)
		throw ConfigException("Invalid Redirection");
	if (key != 300 && key != 301 && key != 302 && key != 303 && key != 304 && key != 307 && key != 308)
		throw ConfigException("Invalid Redirection");
	if (path.substr(0, 2) == "./")
		throw ConfigException("Redirection can not call itself");
	
	int isValid = 0;
	if (path.find("http://") == 0)
	{
		for (size_t i = 0; i < server.getVecPort().size(); i++)
		{
			std::ostringstream oss2;
			oss2 << server.getVecPorti(i);
			std::string str_post= oss2.str();
			std::string url_tmp = "http://" + server.getStrHost() + ":" + str_post + "/";
			if (path.find(url_tmp) == 0 && server.getStrHost() != "localhost" && server.getStrHost() != "127.0.0.1")
			{
				isValid = 1;
			}
			else if (server.getStrHost() == "localhost" || server.getStrHost() == "127.0.0.1")
			{
				url_tmp = "http://127.0.0.1:" + str_post + "/";
				std::string url_tmp2 = "http://localhost:" + str_post + "/";
				if (path.find(url_tmp) == 0 || path.find(url_tmp2) == 0)
				{
					isValid = 1;
				}
			}
		}
		if (!isValid)
			throw ConfigException("Invalid Redirection");
	}
	else
	{
		if (path.find("//") != path.npos)
			throw ConfigException("Invalid Redirection");
	}
	loc.setRedirectionPath(path);
	loc.setRedirectionCode(key);
	nbRedirection++;
	if (nbRedirection > 1)
		throw ConfigException("Too many Redirection");
	return (0);
}

int Config::fillServer(Server &server)
{
	int nbHost = 0;
	int nbServerName = 0;
	int nbClientMax = 0;
	int nbRoot = 0;
	int nbIndex = 0;

    for (size_t i = 0; i < serverConfig.size(); i++)
	{
		if (serverConfig[i].find("listen") == 0)
			makePort(server, serverConfig[i]);
		else if (serverConfig[i].find("host") == 0)
			makeHost(server, serverConfig[i], nbHost);
		else if (serverConfig[i].find("server_name") == 0)
			makeServerName(server, serverConfig[i], nbServerName);
		else if (serverConfig[i].find("client_max_body_size") == 0)
			makeClientMax(server, serverConfig[i], nbClientMax);
		else if (serverConfig[i].find("root") == 0)
			makeRoot(server, serverConfig[i], nbRoot);
		else if (serverConfig[i].find("index") == 0)
			makeIndex(server, serverConfig[i], nbIndex);
    }
	fillLocation(server);
	cleanError(server);

	return (0);
}

std::string Config::getValueLoc(std::string line, int &pos)
{
	int	fpos = line.find(' ', pos);
	int	lpos = line.find(' ', fpos + 1);
	std::string str = line.substr(fpos + 1, lpos - fpos - 1);
	if (!str.empty() && str[str.length() - 1] == ';')
		str.erase(str.length() - 1);
	pos = lpos;
	return (str);
}

int Config::missElementLoc(Location &loc, Server &server)
{
	if (loc.getRoot().length() == 0 && loc.getRedirectionPath().size() == 0)
		throw ConfigException("Missing Root");
	if (loc.getMethod().size() == 0)
		throw ConfigException("Missing Method");
	if (!loc.getIndex().empty())
		if (loc.getIndex().size() == 0 && server.getIndex().size() == 0 && loc.getAutoindex() != 1 && loc.getPath() != "/cgi-bin")
			throw ConfigException("Missing Index");
	return (0);
}

int Config::missElementCgi(Location &loc)
{
	if (loc.getCgiEx().size() == 0)
		throw ConfigException("Missing Cgi");
	if (loc.getCgiPath().size() == 0)
		throw ConfigException("Missing Cgi Path");
	if (loc.getRedirectionPath().size() != 0)
		throw ConfigException("Param return not allow for CGI");
	return (0);
}

int Config::setDefaultMethods(Location &loc)
{
	if (loc.getMethod().size() == 0)
	{
		loc.setMethod("GET");
		loc.setMethod("POST");
		loc.setMethod("DELETE");
	}
	return (0);
}

void changeRedirectionLoc(std::map<std::string, Location> &locations)
{
	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (it->second.getRedirectionPath().size() > 0)
		{
			for (std::map<std::string, Location>::iterator it2 = locations.begin(); it2 != locations.end(); ++it2)
			{
				if (it != it2)
				{
					if (it->second.getRedirectionPath() == it2->first && it->second.getRedirectionPath()[it->second.getRedirectionPath().length() - 1] != '/')
					{
						std::string tmp = it->second.getRedirectionPath() + "/";
						it->second.setRedirectionPath(tmp); 
					}
				}
			}
		}
	}
}

int	Config::fillLocation(Server &server)
{
	size_t i = 0;

	if (location.size() == 0)
	{
		Location loc;
		if (server.getRoot().length() == 0 || server.getIndexi(0).length() == 0)
			throw ConfigException("Missing Root or Index");
		loc.setPath("/");
		loc.setRoot(server.getRoot());
		for (size_t i = 0; i < server.getIndex().size(); i++)	{
			loc.setIndex(server.getIndexi(i));
		}		
		setDefaultMethods(loc);
		if (server.setLoc(loc.getPath(), loc))
			throw ConfigException("Duplicate Location");
		return (0);
	}
	while (i < location.size())
	{
		if (location[i].find("location") == 0)
		{
			int nbRoot = 0;
			int nbIndex = 0;
			int nbAutoIndex = 0;
			int nbAllowMethods = 0;
			int nbCgiEx = 0;
			int nbCgiPath = 0;
			int nbRedirection = 0;
			int nbUploadDir = 0;
			Location loc;
			if (loc.setPath(location[i]))
				throw ConfigException("Invalid Path");
			while (location[i] != "}")
			{
				if (location[i].find("root") == 0)
					makeRoot(loc, location[i], nbRoot);
				else if (location[i].find("index") == 0)
					makeIndex(loc, location[i], nbIndex);
				else if (location[i].find("autoindex") == 0 && loc.getPath() != "/cgi-bin")
					makeAutoIndex(loc, location[i], nbAutoIndex);
				else if (location[i].find("autoindex") == 0 && loc.getPath() == "/cgi-bin")
					throw ConfigException("Parametr autoindex not allow for CGI");
				else if (location[i].find("allow_methods") == 0)
					makeMethod(loc, location[i], nbAllowMethods);
				else if (location[i].find("cgi_extension") == 0)
					makeCgiEx(loc, location[i], nbCgiEx);
				else if (location[i].find("cgi_path") == 0)
					makeCgiPath(loc, location[i], nbCgiPath);
				else if (location[i].find("return") == 0)
					makeRedirection(loc, location[i], nbRedirection, server);
				else if (location[i].find("upload_dir") == 0)
					makeUploadDir(loc, location[i], nbUploadDir);
				i++;
			}
			if (loc.getMethod().size() == 0)
				setDefaultMethods(loc);
			if (loc.getRoot().size() == 0 && server.getRoot().size() != 0 && loc.getRedirectionPath().size() == 0)
				loc.setRoot(server.getRoot());
			if (!loc.getIndex().empty())	{
				if (loc.getIndex().size() == 0 && loc.getAutoindex() != 1)	{
					for (size_t i = 0; i < server.getIndex().size(); i++)	{
						loc.setIndex(server.getIndexi(i));
					}		
				}
			}
			missElementLoc(loc, server);
			if (loc.getPath() == "/cgi-bin")
				missElementCgi(loc);
			if (server.setLoc(loc.getPath(), loc))
				throw ConfigException("Duplicate Location");
		}
		i++;
	}
	changeRedirectionLoc(server.getLoc());
	return (0);
}
