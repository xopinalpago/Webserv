#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

int Config::getNbConfig(void)
{
	return (this->nb_config);
}

void Config::setNbConfig(std::string &filename, std::string to_find)
{
    std::ifstream file(filename.c_str());
    std::string line;
    int count = 0;

    if (file.is_open())
	{
        while (getline(file, line))
		{
            size_t pos = 0;
            while ((pos = line.find(to_find, pos)) != std::string::npos)
			{
                count++;
                pos += to_find.length();
            }
        }
        file.close();
    }
	else
	{
		this->nb_config = 0;
		return ;
    }
	this->nb_config = count;
    return ;
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
				{
					nbErrorPage = 0;
				}
				if (line.find("location") == 0)
				{
					// std::cout << line << std::endl;
                    insideLocation = true;
				}
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
					// std::cout << line << std::endl;
					fillServer(server);
					// if (missElement(server))
					// 	return (1);
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

// int Config::cleanMethod(int serverToRead, Server &server)
// {
// 	int servernb = 0;
//     for (size_t i = 0; i < method.size(); i++)
// 	{
// 		if (method[i].length() > 0)
// 		{
// 			if (method[i].find("method ") == 0)
// 			{
// 				servernb += 1;
// 			}
// 			if (servernb == serverToRead && method[i].find("method {"))
// 			{
// 				std::string tmp_trim = Utils::trim(method[i]);
// 				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
// 				if (tmp == "GET" || tmp == "HEAD" || tmp == "PATCH" || tmp == "POST" || tmp == "PUT" || tmp == "OPTIONS" || tmp == "DELETE")
// 					server.setMethod(tmp);
// 				else
// 					return (1);
// 			}
// 		}
// 	}
// 	if (server.getMethod().empty())
// 		return (1);
// 	return (0);
// }

// int Config::cleanCGI(int serverToRead, Server &server)
// {
// 	int servernb = 0;
//     for (size_t i = 0; i < cgi_extension.size(); i++)
// 	{
// 		if (cgi_extension[i].length() > 0)
// 		{
// 			if (cgi_extension[i].find("cgi_extension ") == 0)
// 			{
// 				servernb += 1;
// 			}
// 			if (servernb == serverToRead && cgi_extension[i].find("cgi_extension {"))
// 			{
// 				std::string tmp_trim = Utils::trim(cgi_extension[i]);
// 				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
// 				if (tmp == "php" || tmp == "py")
// 					server.setCgiEx(tmp);
// 				else
// 					return (1);
// 			}
// 		}
// 	}
// 	if (server.getCgiEx().empty())
// 		return (1);
// 	return (0);
// }

int Config::cleanError(Server &server)
{
	// int servernb = 0;
    for (size_t i = 0; i < error_page.size(); i++)
	{
		if (error_page[i].length() > 0)
		{
			// if (error_page[i].find("error_page ") == 0)
			// {
			// 	servernb += 1;
			// }
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
	if (server.getRoot().length() == 0)
		return (1);
	if (server.getIndex().length() == 0)
		return (1);
	if (server.getDirectory().length() == 0)
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

int Config::makeRoot(Server &server, std::string str, int &nbRoot)
{
	std::string root = getValue(str);
	if (root.length() == 0)
		return (1);
	if (server.setRoot(root))
		return (1);
	nbRoot++;
	if (nbRoot > 1)
		return (1);
	return (0);
}

int Config::makeIndex(Server &server, std::string str, int &nbIndex)
{
	std::string index = getValue(str);
	if (index.length() == 0)
		return (1);
	if (server.setIndex(index))
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

int Config::makeDirectory(Server &server, std::string str, int &nbDirectory)
{
	std::string directory_listing = getValue(str);
	if (directory_listing.length() == 0)
		return (1);
	if (server.setDirectory(directory_listing))
		return (1);
	nbDirectory++;
	if (nbDirectory > 1)
		return (1);
	return (0);
}

int Config::fillServer(Server &server)
{
	// int servernb = 0;
	int nbPort = 0;
	int nbHost = 0;
	int nbServerName = 0;
	int nbRoot = 0;
	int nbIndex = 0;
	int nbClientMax = 0;
	int nbDirectory = 0;

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
		else if (serverConfig[i].find("root") == 0)
		{
			if (makeRoot(server, serverConfig[i], nbRoot))
				return (1);
		}
		else if (serverConfig[i].find("index") == 0)
		{
			if (makeIndex(server, serverConfig[i], nbIndex))
				return (1);
		}
		else if (serverConfig[i].find("client_max_body_size") == 0)
		{
			if (makeClientMax(server, serverConfig[i], nbClientMax))
				return (1);
		}
		else if (serverConfig[i].find("directory_listing") == 0)
		{
			if (makeDirectory(server, serverConfig[i], nbDirectory))
				return (1);
		}
	// 	}
    }
	// if (cleanMethod(serverToRead, server))
	// 	return (1);
	// if (cleanCGI(serverToRead, server))
	// 	return (1);

	// for (size_t i = 0; i < location.size(); ++i) {
    //     std::cout << location[i] << std::endl;
    // }
	// std::cout << std::endl;

	if (fillLocation(server))
		return (1);

	if (cleanError(server))
		return (1);
	
	
	std::cout << server.getLoci("/test").getMethodi(0) << std::endl;

	return (0);
}

int	Config::fillLocation(Server &server)
{
	size_t i = 0;

	while (i < location.size())
	{
		if (location[i].find("location") == 0)
		{
			Location loc;
			int	fpos = location[i].find(' ', 0);
			int	lpos = location[i].find(' ', fpos + 1);
			loc.setPath(location[i].substr(fpos + 1, lpos - fpos - 1));
			std::string last = location[i].substr(lpos + 1, lpos - location[i].size());
			if (last != "{")
				return (1);
			while (location[i] != "}")
			{
				// std::cout << location[i] << std::endl;
				if (location[i].find("root") == 0)
				{
					if (loc.setRoot(getValue(location[i])))
						return (1);
				}
				else if (location[i].find("index") == 0)
				{
					if (loc.setIndex(getValue(location[i])))
						return (1);
				}
				else if (location[i].find("autoindex") == 0)
				{
					if (loc.setAutoindex(getValue(location[i])))
						return (1);
				}
				else if (location[i].find("allow_methods") == 0)
				{
					int pos = 0;
					while (location[i].find(' ', pos) != location[i].npos)
					{
						int	fpos = location[i].find(' ', pos);
						int	lpos = location[i].find(' ', fpos + 1);
						std::string str = location[i].substr(fpos + 1, lpos - fpos - 1);
						if (!str.empty() && str[str.length() - 1] == ';')
							str.erase(str.length() - 1);
						loc.setMethod(str);
						pos = lpos;
					}
				}
				else if (location[i].find("cgi_extension") == 0)
				{
					int pos = 0;
					while (location[i].find(' ', pos) != location[i].npos)
					{
						int	fpos = location[i].find(' ', pos);
						int	lpos = location[i].find(' ', fpos + 1);
						std::string str = location[i].substr(fpos + 1, lpos - fpos - 1);
						if (!str.empty() && str[str.length() - 1] == ';')
							str.erase(str.length() - 1);
						loc.setCgiEx(str);
						pos = lpos;
					}

				}
				else if (location[i].find("cgi_path") == 0)
				{
					int pos = 0;
					while (location[i].find(' ', pos) != location[i].npos)
					{
						int	fpos = location[i].find(' ', pos);
						int	lpos = location[i].find(' ', fpos + 1);
						int	llpos = location[i].find(' ', lpos + 1);
						std::string key = location[i].substr(fpos + 1, lpos - fpos - 1);
						std::string path = location[i].substr(lpos + 1, llpos - lpos - 1);
						if (!path.empty() && path[path.length() - 1] == ';')
							path.erase(path.length() - 1);
						if (key.empty() || path.empty())
							return (1);
						// std::cout << "key = " << key << std::endl;
						// std::cout << "path = " << path << std::endl; 
						loc.setCgiPath(key, path);
						pos = llpos;
						// std::cout << loc.getCgiPathi(key) << std::endl;
					}
				}
				i++;
			}
			server.setLoc(loc.getPath(), loc);
			// server.locations[loc.getPath()] = loc;
		}
		i++;
	}
	return (0);
}
