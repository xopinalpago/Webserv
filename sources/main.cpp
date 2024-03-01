#include "Server.hpp"
#include "Config.hpp"

int main()
{
	Server server;
	Config config;
	
	config.GetLineFile();
	config.ParseFile();
	if (server.initServer(config.server))
		return (1);
	if (server.runServer())
		return (1);	
    return (0);
}
