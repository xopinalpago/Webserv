#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

// int Config::getNbConfig(void)
// {
// 	return (this->nb_config);
// }

// void Config::setNbConfig(std::string &filename, std::string to_find)
// {
//     std::ifstream file(filename.c_str());
//     std::string line;
//     int count = 0;

//     if (file.is_open())
// 	{
//         while (getline(file, line))
// 		{
//             size_t pos = 0;
//             while ((pos = line.find(to_find, pos)) != std::string::npos)
// 			{
//                 count++;
//                 pos += to_find.length();
//             }
//         }
//         file.close();
//     }
// 	else
// 	{
// 		this->nb_config = 0;
// 		return ;
//     }
// 	this->nb_config = count;
//     return ;
// }

int Config::checkLocation(Server &server)
{
    std::vector<std::string> keys;

	if (server.getLoc().size() == 1)
		return (0);
    for (std::map<std::string, Location>::iterator it = server.getLoc().begin(); it != server.getLoc().end(); ++it)
    {
        keys.push_back(it->first);
    }

    for (size_t i = 0; i < keys.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < keys.size(); ++j)
        {
            std::string key1 = keys[i];
            std::string key2 = keys[j];
			std::cout << "key1 = " << key1 << std::endl;
			std::cout << "key2 = " << key2 << std::endl;
            if (server.getLoc()[key1].getPath() == server.getLoc()[key2].getPath())
            {
                return (1);
            }
        }
    }
    return (0);
}

int Config::getLineFile(std::string &filename, Launcher &launcher)
{
    std::ifstream infile(filename.c_str());
	// int nbMethod = 0;
	int nbErrorPage = 0;
	

    if (infile)
	{
        std::string line;
        bool insideLocation = false;
		bool insideError = false; 
        while (std::getline(infile, line))
		{
            line = Utils::trim(line);
			line = Utils::trimTab(line);
			// std::cout << line << std::endl; 
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

    // Affichage du contenu du vecteur
    // for (size_t i = 0; i < location.size(); ++i) {
    //     std::cout << location[i] << std::endl;
    // }

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
	if (server.getErrorPage().empty())
		throw ConfigException("No Error Page");
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
	if (server.getPort() <= 0)
		throw ConfigException("No Port");
	if (server.getClientMax() <= 0)
		throw ConfigException("No Client Max Body Size");
	if (server.getHost() <= 0)
		throw ConfigException("No Host");
	return (0);
}

int Config::makePort(Server &server, std::string str, int &nbPort)
{
	int port = Utils::stringToInt(getValue(str));
	if (port <= 0)
		throw ConfigException("Invalid Port");
	if (server.setPort(port))
		throw ConfigException("Invalid Port");
	nbPort++;
	if (nbPort > 1)
		throw ConfigException("Too many Port");
	return (0);
}

int Config::makeHost(Server &server, std::string str, int &nbHost)
{
	std::string host = getValue(str);
	if (host.length() == 0)
		throw ConfigException("Invalid Host");
	if (server.setHost(host))
		throw ConfigException("Invalid Host");
	nbHost++;
	if (nbHost > 1)
		throw ConfigException("Too many Host");
	return (0);
}

int Config::makeServerName(Server &server, std::string str, int &nbServerName)
{
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
	std::string root = getValue(str);
	if (root.length() == 0)
		throw ConfigException("Invalid Root");
	if (loc.setRoot(root))
		throw ConfigException("Invalid Root");
	nbRoot++;
	if (nbRoot > 1)
		throw ConfigException("Too many Root");
	return (0);
}

int Config::makeIndex(Location &loc, std::string str, int &nbIndex)
{
	std::string index = getValue(str);
	if (index.length() == 0)
		throw ConfigException("Invalid Index");
	if (loc.setIndex(index))
		throw ConfigException("Invalid Index");
	nbIndex++;
	if (nbIndex > 1)
		throw ConfigException("Too many Index");
	return (0);
}

int Config::makeClientMax(Server &server, std::string str, int &nbClientMax)
{
	int client_max_body_size = Utils::stringToInt(getValue(str));
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
	std::string index = getValue(str);
	if (index.length() == 0)
		throw ConfigException("Invalid Index");
	if (server.setIndex(index))
		throw ConfigException("Invalid Index");
	nbIndex++;
	if (nbIndex > 1)
		throw ConfigException("Too many Index");
	return (0);
}

int Config::makeAutoIndex(Location &loc, std::string str, int &nbAutoIndex)
{
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
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string method = getValueLoc(str, pos);
		if (method.length() == 0)
			throw ConfigException("Invalid Method");
		loc.setMethod(method);
	}
	nbAllowMethods++;
	if (nbAllowMethods > 1)
		throw ConfigException("Too many Method");
	return (0);
}

int Config::makeCgiEx(Location &loc, std::string str, int &nbCgiEx)
{
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
	int pos = 0;
	if (str.find(' ', pos) == str.npos)
		return (1);
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
		loc.setCgiPath(key, path);
		pos = llpos;
	}
	nbCgiPath++;
	if (nbCgiPath > 1)
		throw ConfigException("Too many Cgi Path");
	return (0);
}

int Config::fillServer(Server &server)
{
	int nbPort = 0;
	int nbHost = 0;
	int nbServerName = 0;
	int nbClientMax = 0;
	int nbRoot = 0;
	int nbIndex = 0;

    for (size_t i = 0; i < serverConfig.size(); i++)
	{
		if (serverConfig[i].find("listen") == 0)
			makePort(server, serverConfig[i], nbPort);
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
	// std::cout << server.getLoci("/test").getMethodi(0) << std::endl;

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

int Config::missElementLoc(Location &loc)
{
	if (loc.getRoot().length() == 0)
		throw ConfigException("Missing Root");
	if (loc.getMethod().size() == 0)
		throw ConfigException("Missing Method");
	return (0);
}

int Config::missElementCgi(Location &loc)
{
	if (loc.getCgiEx().size() == 0)
		throw ConfigException("Missing Cgi");
	if (loc.getCgiPath().size() == 0)
		throw ConfigException("Missing Cgi Path");
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

int	Config::fillLocation(Server &server)
{
	size_t i = 0;

	if (location.size() == 0)
	{
		Location loc;
		if (server.getRoot().length() == 0 || server.getIndex().length() == 0)
			throw ConfigException("Missing Root or Index");
		loc.setPath("/");
		loc.setRoot(server.getRoot());
		loc.setIndex(server.getIndex());
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
			Location loc;
			if (loc.setPath(location[i]))
				throw ConfigException("Invalid Path");
			while (location[i] != "}")
			{
				if (location[i].find("root") == 0)
					makeRoot(loc, location[i], nbRoot);
				else if (location[i].find("index") == 0)
					makeIndex(loc, location[i], nbIndex);
				else if (location[i].find("autoindex") == 0)
					makeAutoIndex(loc, location[i], nbAutoIndex);
				else if (location[i].find("allow_methods") == 0)
					makeMethod(loc, location[i], nbAllowMethods);
				else if (location[i].find("cgi_extension") == 0)
					makeCgiEx(loc, location[i], nbCgiEx);
				else if (location[i].find("cgi_path") == 0)
					makeCgiPath(loc, location[i], nbCgiPath);
				i++;
			}
			if (loc.getMethod().size() == 0)
				setDefaultMethods(loc);
			if (loc.getRoot().size() == 0 && server.getRoot().size() != 0)
				loc.setRoot(server.getRoot());
			missElementLoc(loc);
			if (loc.getPath() == "/cgi-bin")
				missElementCgi(loc);
			if (server.setLoc(loc.getPath(), loc))
				throw ConfigException("Duplicate Location");
		}
		i++;
	}
	return (0);
}
