#include "Server.hpp"
#include "Config.hpp"
#include "Launcher.hpp"

int main(int argc, char **argv)
{
	Launcher 	run;

	if (argc == 2)
	{
		std::string filename(argv[1]);
		if (run.initConfig(filename))
			return (1);
		if (run.runServer())
			return (1);	
	}
    return (0);
}
