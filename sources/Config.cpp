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
					if (fillServer(server))
						return (1);
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
        return (1);
    }

    // Affichage du contenu du vecteur
    // for (size_t i = 0; i < location.size(); ++i) {
    //     std::cout << location[i] << std::endl;
    // }

    return (0);
}

int Config::cleanError(Server &server)
{
    for (size_t i = 0; i < error_page.size(); i++)
	{
		if (error_page[i].length() > 0)
		{
			if (error_page[i].find("error_page {"))
			{
				if (!std::isdigit(error_page[i][0]))
					return (1);
				int first_sp = error_page[i].find(' ');
				if (first_sp < 0)
					return (1);
				int error_num = Utils::stringToInt(error_page[i].substr(0, first_sp));
				//TESTER SI ERROR EST DANS LES ERREURS QU ON GERE
				std::string tmp_trim = Utils::trim(error_page[i]);
				std::string error_file = tmp_trim.substr(first_sp + 1, tmp_trim.size() - first_sp - 2);
				if (Utils::fileExists(error_file))
					return (1);
				server.setErrorPage(error_num, error_file); 
			}
		}
	}
	if (server.getErrorPage().empty())
		return (1);
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
		return (1);
	if (server.getClientMax() <= 0)
		return (1);
	if (server.getHost() <= 0)
		return (1);
	return (0);
}

int Config::makePort(Server &server, std::string str, int &nbPort)
{
	int port = Utils::stringToInt(getValue(str));
	if (port <= 0)
		return (1);
	if (server.setPort(port))
		return (1);
	nbPort++;
	if (nbPort > 1)
		return (1);
	return (0);
}

int Config::makeHost(Server &server, std::string str, int &nbHost)
{
	std::string host = getValue(str);
	if (host.length() == 0)
		return (1);
	if (server.setHost(host))
		return (1);
	nbHost++;
	if (nbHost > 1)
		return (1);
	return (0);
}

int Config::makeServerName(Server &server, std::string str, int &nbServerName)
{
	std::string server_name = getValue(str);
	if (server_name.length() == 0)
		return (1);
	if (server.setServerName(server_name))
		return (1);
	nbServerName++;
	if (nbServerName > 1)
		return (1);
	return (0);
}

int Config::makeRoot(Location &loc, std::string str, int &nbRoot)
{
	std::string root = getValue(str);
	if (root.length() == 0)
		return (1);
	if (loc.setRoot(root))
		return (1);
	nbRoot++;
	if (nbRoot > 1)
		return (1);
	return (0);
}

int Config::makeIndex(Location &loc, std::string str, int &nbIndex)
{
	std::string index = getValue(str);
	if (index.length() == 0)
		return (1);
	if (loc.setIndex(index))
		return (1);
	nbIndex++;
	if (nbIndex > 1)
		return (1);
	return (0);
}

int Config::makeClientMax(Server &server, std::string str, int &nbClientMax)
{
	int client_max_body_size = Utils::stringToInt(getValue(str));
	if (client_max_body_size <= 0)
		return (1);
	if (server.setClientMax(client_max_body_size))
		return (1);
	nbClientMax++;
	if (nbClientMax > 1)
		return (1);
	return (0);
}

int Config::makeAutoIndex(Location &loc, std::string str, int &nbAutoIndex)
{
	std::string autoIndex = getValue(str);
	if (autoIndex.length() == 0)
		return (1);
	if (loc.setAutoindex(autoIndex))
		return (1);
	nbAutoIndex++;
	if (nbAutoIndex > 1)
		return (1);
	return (0);
}

int Config::makeMethod(Location &loc, std::string str, int &nbAllowMethods)
{
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string method = getValueLoc(str, pos);
		if (method.length() == 0)
			return (1);
		loc.setMethod(method);
	}
	nbAllowMethods++;
	if (nbAllowMethods > 1)
		return (1);
	return (0);
}

int Config::makeCgiEx(Location &loc, std::string str, int &nbCgiEx)
{
	int pos = 0;
	while (str.find(' ', pos) != str.npos)
	{
		std::string cigEx = getValueLoc(str, pos);
		if (cigEx.length() == 0)
			return (1);
		loc.setCgiEx(cigEx);
	}
	nbCgiEx++;
	if (nbCgiEx > 1)
		return (1);
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
			return (1);
		loc.setCgiPath(key, path);
		pos = llpos;
	}
	nbCgiPath++;
	if (nbCgiPath > 1)
		return (1);
	return (0);
}

int Config::fillServer(Server &server)
{
	int nbPort = 0;
	int nbHost = 0;
	int nbServerName = 0;
	int nbClientMax = 0;

    for (size_t i = 0; i < serverConfig.size(); i++)
	{
		if (serverConfig[i].find("listen") == 0)
		{
			if (makePort(server, serverConfig[i], nbPort))
				return (1);
		}
		else if (serverConfig[i].find("host") == 0)
		{
			if (makeHost(server, serverConfig[i], nbHost))
				return (1);
		}
		else if (serverConfig[i].find("server_name") == 0)
		{
			if (makeServerName(server, serverConfig[i], nbServerName))
				return (1);
		}
		else if (serverConfig[i].find("client_max_body_size") == 0)
		{
			if (makeClientMax(server, serverConfig[i], nbClientMax))
				return (1);
		}
    }
	if (fillLocation(server))
		return (1);
	if (cleanError(server))
		return (1);
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

int Config::missElementLocRoot(Location &loc)
{
	if (loc.getRoot().length() == 0)
		return (1);
	if (loc.getIndex().length() == 0)
		return (1);
	if (loc.getMethod().size() == 0)
		return (1);
	return (0);
}

int	Config::fillLocation(Server &server)
{
	size_t i = 0;

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
				return (1);
			while (location[i] != "}")
			{
				if (location[i].find("root") == 0)
				{
					if (makeRoot(loc, location[i], nbRoot))
						return (1);
				}
				else if (location[i].find("index") == 0)
				{
					if (makeIndex(loc, location[i], nbIndex))
						return (1);
				}
				else if (location[i].find("autoindex") == 0)
				{
					if (makeAutoIndex(loc, location[i], nbAutoIndex))
						return (1);
				}
				else if (location[i].find("allow_methods") == 0)
				{
					if (makeMethod(loc, location[i], nbAllowMethods))
						return (1);
				}
				else if (location[i].find("cgi_extension") == 0)
				{
					if (makeCgiEx(loc, location[i], nbCgiEx))
						return (1);
				}
				else if (location[i].find("cgi_path") == 0)
				{
					if (makeCgiPath(loc, location[i], nbCgiPath))
						return (1);
				}
				i++;
			}
			if (loc.getPath() == "/")
				if (missElementLocRoot(loc))
					return (1);
			if (server.setLoc(loc.getPath(), loc))
				return (1);
			// server.locations[loc.getPath()] = loc;
		}
		i++;
	}
	return (0);
}
