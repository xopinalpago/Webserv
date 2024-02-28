#include "Server.hpp"

int main()
{
	Server server;

	if (server.initServer())
		return (1);
	if (server.runServer())
		return (1);	
    return (0);
}
