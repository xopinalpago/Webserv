#include "Server.hpp"
#include "Config.hpp"
#include "Launcher.hpp"

void	handle_sigint(int sig)
{
	throw Launcher::SigError();
	(void)sig;
}

int main(int argc, char **argv)
{
	Launcher 	run;

	if (argc == 2)
	{
		try {
			signal(SIGINT, &handle_sigint);
			std::string filename(argv[1]);
			if (run.initConfig(filename))
				return (1);
			if (run.runServer())
				return (1);
		}
		catch (Launcher::SigError &e) {
			run.closeAllConnection();
			return (0);
		}
	}
    return (0);
}
