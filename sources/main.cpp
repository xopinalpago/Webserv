#include "Server.hpp"
#include "Config.hpp"

int main()
{
	Config config;
	Server servers;
	config.nb_config = config.countOccurrences("server.conf", "server ");
	// std::cout << config.nb_config << std::endl;
	config.GetLineFile();
	for (int i = 1; i <= config.nb_config; i++)
	{
		Server server;
		config.ParseFile(i, server);
		std::cout << server.getPort() << std::endl;
		if (servers.initServer(server))
			return (1);
	}
	
	if (servers.runServer())
		return (1);	
    return (0);
}
