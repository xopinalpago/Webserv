#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

std::string Config::trim(std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

int Config::GetNbConfig(std::string &filename, std::string to_find)
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
		return (0);
    }
    return (count);
}

int Config::GetLineFile(std::string &filename)
{
    std::ifstream infile(filename.c_str());

    if (infile) {
        std::string line;
        bool insideMethod = false;
        bool insideCgi = false;
		bool insideError = false; 
        while (std::getline(infile, line)) {
            line = trim(line);
            if (!line.empty())
			{
				if (line.find("method") == 0)
                    insideMethod = true;
				else if (line == "}" && insideMethod)
                    insideMethod = false;
				else if (line.find("cgi_extension") == 0)
					insideCgi = true;
				else if (line == "}" && insideCgi)
                    insideCgi = false;
				else if (line.find("error_page") == 0)
					insideError = true;
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
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return (1);
    }

    // Affichage du contenu du vecteur
    // for (size_t i = 0; i < error_page.size(); ++i) {
    //     std::cout << error_page[i] << std::endl;
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
				std::string tmp_trim = trim(method[i]);
				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
				if (tmp == "GET" || tmp == "HEAD" || tmp == "PATCH" || tmp == "POST" || tmp == "PUT" || tmp == "OPTIONS" || tmp == "DELETE")
					server.method.push_back(tmp);
				else
					return (1);
			}
		}
	}
	if (server.method.empty())
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
				std::string tmp_trim = trim(cgi_extension[i]);
				std::string tmp = tmp_trim.substr(0, tmp_trim.size() - 1);
				if (tmp == "php" || tmp == "py")
					server.cgi_extension.push_back(tmp);
				else
					return (1);
			}
		}
	}
	if (server.cgi_extension.empty())
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
				std::string tmp_trim = trim(error_page[i]);
				std::string error_file = tmp_trim.substr(first_sp + 1, tmp_trim.size() - first_sp - 2);
				if (Utils::fileExists(error_file))
					return (1);
				server.error_page[error_num] = error_file; 
			}
		}
	}
	if (server.error_page.empty())
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
	if (server.getHost().length() == 0)
		return (1);
	if (server.getServerName().length() == 0)
		return (1);
	if (server.getRoot().length() == 0)
		return (1);
	if (server.getIndex().length() == 0)
		return (1);
	if (server.getClientMax().length() == 0)
		return (1);
	if (server.getDirectory().length() == 0)
		return (1);
	return (0);
}

int Config::ParseFile(int serverToRead, Server &server)
{
	int servernb = 0;
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
				int port = Utils::stringToInt(getValue(serverConfig[i]));
				if (port <= 0)
					return (1);
				if (server.setPort(port))
					return (1);
			}
			else if (serverConfig[i].find("host") == 0)
			{
				std::string host = getValue(serverConfig[i]);
				if (host.length() == 0)
					return (1);
				if (server.setHost(host))
					return (1);
			}
			else if (serverConfig[i].find("server_name") == 0)
			{
				std::string server_name = getValue(serverConfig[i]);
				if (server_name.length() == 0)
					return (1);
				if (server.setServerName(server_name))
					return (1);
			}
			else if (serverConfig[i].find("root") == 0)
			{
				std::string root = getValue(serverConfig[i]);
				if (root.length() == 0)
					return (1);
				if (server.setRoot(root))
					return (1);
			}
			else if (serverConfig[i].find("index") == 0)
			{
				std::string index = getValue(serverConfig[i]);
				if (index.length() == 0)
					return (1);
				if (server.setIndex(index))
					return (1);
			}
			else if (serverConfig[i].find("client_max_body_size") == 0)
			{
				std::string client_max_body_size = getValue(serverConfig[i]);
				if (client_max_body_size.length() == 0)
					return (1);
				if (server.setClientMax(client_max_body_size))
					return (1);
			}
			else if (serverConfig[i].find("directory_listing") == 0)
			{
				std::string directory_listing = getValue(serverConfig[i]);
				if (directory_listing.length() == 0)
					return (1);
				if (server.setDirectory(directory_listing))
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

	// for (size_t i = 0; i < server.cgi_extension.size(); ++i) {
    //     std::cout << server.cgi_extension[i] << " ";
    // }
    // std::cout << std::endl;
	return (0);
}
