#include "Server.hpp"
#include "Config.hpp"
#include "Launcher.hpp"

void	handle_sigint(int sig)
{
	throw Launcher::SigError();
	(void)sig;
}

void	handle_sigpipe(int sig)
{
	(void)sig;
}

int main(int argc, char **argv)
{
	Launcher 	run;
	Config		config;

	if (argc <= 2)
	{
		std::string filename;
		if (argc == 1)
			filename = "server.conf";
		else
			filename = argv[1];
		try {
			signal(SIGINT, &handle_sigint);
			signal(SIGPIPE, &handle_sigpipe);
			config.getLineFile(filename, run);
			run.runServer();
		}
		catch (Launcher::SigError &e) {
			run.closeAllConnection();
			return (0);
		}
		catch (Config::ConfigException &e) {
			run.closeAllConnection();
			std::cerr << e.what() << std::endl;
			return (0);
		}
		catch (Launcher::LauncherException &e) {
			std::cerr << e.what() << std::endl;
			run.closeAllConnection();
			return (0);
		}
	}
    return (0);
}
