#include "Config.hpp"

Config::Config(void) {}

Config::~Config(void) {}

std::string Config::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int Config::GetLineFile(void)
{
    std::ifstream infile("server.conf"); // Assurez-vous que le fichier est dans le même répertoire que votre exécutable

    if (infile) {
        std::string line;
        bool insideMethod = false; // Pour savoir si nous sommes à l'intérieur de la section "method"
        bool insideCgi = false; // Pour savoir si nous sommes à l'intérieur de la section "method"
        while (std::getline(infile, line)) {
            line = trim(line); // Supprimer les espaces au début et à la fin de la ligne
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
				if (!insideMethod && !insideCgi) // Si nous ne sommes pas à l'intérieur de "method"
                    serverConfig.push_back(line); // Ajouter la ligne au vecteur
				else if (insideMethod)
					method.push_back(line);
				else if (insideCgi)
					cgi_extension.push_back(line);
            }
        }
        infile.close();
    } else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return 1;
    }

    // Affichage du contenu du vecteur
    // for (size_t i = 0; i < serverConfig.size(); ++i) {
    //     std::cout << serverConfig[i] << std::endl;
    // }

    return 0;
}

int	Config::StringToInt(std::string str) {

	if (str.length() > 10)
		return (-42);
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (std::isdigit(str[i] == 0))
			return (-42);
	}
	std::stringstream stream(str);
	int n;
	stream >> n;
	return (n);
}

int Config::ParseFile(void)
{
    for (size_t i = 0; i < serverConfig.size(); ++i) {
		if (serverConfig[i].find("listen") == 0)
		{
			int first_sp = serverConfig[i].find(' ');
			server.setPort(StringToInt((serverConfig[i].substr(first_sp + 1, serverConfig[i].size() - 2 - first_sp))));	
		}
		else if (serverConfig[i].find("host") == 0)
		{
			int first_sp = serverConfig[i].find(' ');
			server.setHost(serverConfig[i].substr(first_sp + 1, serverConfig[i].size() - 2 - first_sp));	
		}
    }
	return (1);
}
