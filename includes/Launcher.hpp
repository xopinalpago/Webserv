#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <map>
#include <vector>

#include "Config.hpp"
#include "Server.hpp"
#include "User.hpp"

class Launcher
{
	private:
		struct timeval      timeout;
		// struct sockaddr_in address;
		fd_set	            readfds;
        fd_set	            writefds;
        fd_set	            tmp_readfds;
        fd_set	            tmp_writefds;
        int                 rc;
        int                 max_sd;
        int                 new_sd;
        int                 end_server;

		void 	errorFunction(std::string word);
		int		readServer(User &user);
        void	sendServer(User &user);
        void    listenServer(Server &server);
		int 	initSets(void);
		int 	initServer(Server &server);
		void 	getUserServer(User &user);

	public:
		Launcher(void);
		~Launcher(void);

		int initConfig(std::string &filename);
		int runServer(void);

		std::map<int, User> 	Users;
		std::map<int, Server> 	Servers;
};



#endif