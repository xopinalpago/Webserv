#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <map>
#include <vector>

#include "Server.hpp"
#include "User.hpp"
#include "Request.hpp"


class Launcher
{
	private:
		struct timeval      timeout;
		fd_set	            readfds;
        fd_set	            writefds;
        fd_set	            tmp_readfds;
        fd_set	            tmp_writefds;
        int                 rc;
        int                 max_sd;
        int                 new_sd;
        int                 end_server;
		int					totalBytes;

		int		readServer(User &user);
		int		readbuf(User &user, Request& request, char *bf);
		std::string	extractBody(Request& request);
        void	sendServer(User &user);
        void    listenServer(Server &server);
		int 	initSets(void);
		int		checkServers(void);
		void 	checkServerName(void);
		void    closeConnection(int i);
		void	checkTimeout(void);
		void 	addServerOfClient(int listen_sock, User *client);
	public:
		Launcher(void);
		~Launcher(void);
		Launcher(const Launcher& cpy);
        Launcher& operator=(const Launcher& rhs);

		int 	initServer(Server &server);
		int 	initServer(Server &server, int port);
		int 	initConfig(std::string &filename);
		int 	runServer(void);
		void	closeAllConnection(void);

		std::map<int, User> 	Users;
		std::map<int, Server> 	Servers;

		class LauncherException : public std::exception {
		public :
			LauncherException(std::string err) throw() {
				_err = "Launcher Error: " + err;
			}
			virtual const char* what() const throw() {
				return (_err.c_str());
			}
			~LauncherException() throw() {}
		
		private:
			std::string _err;
		};
		class LauncherInitException : public std::exception {
		public :
			LauncherInitException(std::string err) throw() {
				_err = "Launcher Error: " + err;
			}
			virtual const char* what() const throw() {
				return (_err.c_str());
			}
			~LauncherInitException() throw() {}
		
		private:
			std::string _err;
		};
		class SigError : public std::exception {};
};

#endif