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
	Config		config;

	if (argc == 2)
	{
		try {
			signal(SIGINT, &handle_sigint);
			std::string filename(argv[1]);
			config.getLineFile(filename, run);
			if (run.runServer())
				return (1);
		}
		catch (Launcher::SigError &e) {
			run.closeAllConnection();
			return (0);
		}
		catch (Config::ConfigException &e) {
			std::cerr << e.what() << std::endl;
			return (0);
		}
	}
    return (0);
}
