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

    if (file.is_open()) {
        while (getline(file, line)) {
            size_t pos = 0;
            while ((pos = line.find(to_find, pos)) != std::string::npos) {
                ++count;
                pos += to_find.length();
            }
        }
        file.close();
    } else {
        std::cerr << "Impossible d'ouvrir le fichier." << std::endl;
		this->nb_config = 0;
		return ;
    }
	this->nb_config = count;
    return ;
}

int Config::GetLineFile(std::string &filename)
{
    std::ifstream infile(filename.c_str());
	int nbMethod = 0;
	int nbCgiEx = 0;
	int nbErrorPage = 0;

    if (infile) {
        std::string line;
        bool insideMethod = false;
        bool insideCgi = false;
		bool insideError = false; 
        while (std::getline(infile, line)) {
            line = Utils::trim(line);
            if (!line.empty())
			{
				if (line.find("method") == 0)
				{
					nbMethod++;
					if (nbMethod > 1)
						return (1);
                    insideMethod = true;
				}
				else if (line == "}" && insideMethod)
                    insideMethod = false;
				else if (line.find("cgi_extension") == 0)
				{
					nbCgiEx++;
					if (nbCgiEx > 1)
						return (1);
					insideCgi = true;
				}
				else if (line == "}" && insideCgi)
                    insideCgi = false;
				else if (line.find("error_page") == 0)
				{
					nbErrorPage++;
					if (nbErrorPage > 1)
						return (1);
					insideError = true;
				}
				else if (line == "}" && insideError)
                    insideError = false;              
				if (!insideMethod && !insideCgi && !insideError) 
                    serverConfig.push_back(line);
				else if (insideMethod)
					method.push_back(line);
				else if (insideCgi)
					cgi_extension.push_back(line);
				else if (insideError)
					error_page.push_back(line);
            }
        }
        infile.close();
    } else {
        std::cout << "Erreur lors de l'ouverture du fichier." << std::endl;
        return (1);
    }

    // Affichage du contenu du vecteur
    // for (size_t i = 0; i < cgi_extension.size(); ++i) {
    //     std::cout << cgi_extension[i] << std::endl;
    // }

    return (0);
}

int Config::cleanMethod(int serverToRead, Server &server)
{
	int servernb = 0;
    for (size_t i = 0; i < method.size(); i++)
	{
		if (method[i].length() > 0)
		{
			if (method[i].find("method ") == 0)
			{
				servernb += 1;
			}
			if (servernb == serverToRead && method[i].find("method {"))
			{
				std::string tmp_trim = Utils::trim(method[i]);
				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
				if (tmp == "GET" || tmp == "HEAD" || tmp == "PATCH" || tmp == "POST" || tmp == "PUT" || tmp == "OPTIONS" || tmp == "DELETE")
					server.setMethod(tmp);
				else
					return (1);
			}
		}
	}
	if (server.getMethod().empty())
		return (1);
	return (0);
}

int Config::cleanCGI(int serverToRead, Server &server)
{
	int servernb = 0;
    for (size_t i = 0; i < cgi_extension.size(); i++)
	{
		if (cgi_extension[i].length() > 0)
		{
			if (cgi_extension[i].find("cgi_extension ") == 0)
			{
				servernb += 1;
			}
			if (servernb == serverToRead && cgi_extension[i].find("cgi_extension {"))
			{
				std::string tmp_trim = Utils::trim(cgi_extension[i]);
				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
				if (tmp == "php" || tmp == "py")
					server.setCgiEx(tmp);
				else
					return (1);
			}
		}
	}
	if (server.getCgiEx().empty())
		return (1);
	return (0);
}

int Config::cleanError(int serverToRead, Server &server)
{
	int servernb = 0;
    for (size_t i = 0; i < error_page.size(); i++)
	{
		if (error_page[i].length() > 0)
		{
			if (error_page[i].find("error_page ") == 0)
			{
				servernb += 1;
			}
			if (servernb == serverToRead && error_page[i].find("error_page {"))
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

std::string getValue(std::string line)
{
	int first_sp = line.find(' ');
	if (first_sp < 0)
		return ("");
	return(line.substr(first_sp + 1, line.size() - 2 - first_sp));
}

int Config::MissElement(Server &server)
{
	if (server.getRoot().length() == 0)
		return (1);
	if (server.getIndex().length() == 0)
		return (1);
	if (server.getDirectory().length() == 0)
		return (1);
	return (0);
}

int makePort(Server &server, std::string str, int &nbPort)
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

int makeHost(Server &server, std::string str, int &nbHost)
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

int makeServerName(Server &server, std::string str, int &nbServerName)
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

int makeRoot(Server &server, std::string str, int &nbRoot)
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

int makeIndex(Server &server, std::string str, int &nbIndex)
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

int makeClientMax(Server &server, std::string str, int &nbClientMax)
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

int makeDirectory(Server &server, std::string str, int &nbDirectory)
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

int Config::ParseFile(int serverToRead, Server &server)
{
	int servernb = 0;
	int nbPort = 0;
	int nbHost = 0;
	int nbServerName = 0;
	int nbRoot = 0;
	int nbIndex = 0;
	int nbClientMax = 0;
	int nbDirectory = 0;

    for (size_t i = 0; i < serverConfig.size(); ++i)
	{
		if (serverConfig[i].find("server ") == 0)
		{
			servernb += 1;
		}
		if (servernb == serverToRead)
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
		}
    }
	if (cleanMethod(serverToRead, server))
		return (1);
	if (cleanCGI(serverToRead, server))
		return (1);
	if (cleanError(serverToRead, server))
		return (1);

	// for (size_t i = 0; i < server.getCgiEx().size(); ++i) {
    //     std::cout << server.getCgiExi(i) << " ";
    // }
    // std::cout << std::endl;
	return (0);
}
