#include "Cgi.hpp"

Cgi::Cgi(void) {}

Cgi::~Cgi(void) {}


void Cgi::create_env() {
    _env["SERVER_NAME"] = "Localhost"; // conf ou user
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = "GET"; // user
    _env["SCRIPT_FILENAME"] = "test.php"; // user
    _env["CONTENT_TYPE"] = "text/html"; // user
    _env["CONTENT_LENGTH"] = "882"; // user
    _env["QUERY_STRING"] = "name=Ducobu&firstname=Aurore"; // user
    _env["REQUEST_URI"] = "uri";
    this->mapToChar();
}

int Cgi::mapToChar() {

    memset(&_cenv, 0, sizeof(_cenv));
    std::map<std::string, std::string>::iterator it;
    _cenv = (char**)malloc(sizeof(char*) * (_env.size() + 1));
    if (!_cenv)
        return 1;
    int i = 0;
    for (it = _env.begin() ; it != _env.end() ; ++it) {
        int len = (it->first).length() + 1 + (it->second).length() + 1;
        _cenv[i] = (char*)malloc(sizeof(char) * len);
        if (!_cenv[i])
            return 1;
        strcpy(_cenv[i], (it->first).c_str());
        strcat(_cenv[i], "=");
        strcat(_cenv[i], (it->second).c_str());
        i++;
    }
    _cenv[i] = NULL;
    return 0;
}

void Cgi::displayEnv() {

    for (size_t i = 0; i < _env.size(); i++) {
        std::cout << _cenv[i] << std::endl;
    }
}

void Cgi::freeEnv() {
    for (size_t i = 0; i < _env.size(); i++) {
        free(_cenv[i]);
    }
    free(_cenv);
}


int Cgi::execCGI(std::string file_path) {
    
    /******************************************/
    /******************tmp*********************/
    /******************************************/
    char **args;
    args = (char **)malloc(sizeof(char *) * 3);
    args[0] = (char *)malloc(sizeof(char) * 16);
    args[1] = (char *)malloc(sizeof(char) * 8);
    strcpy(args[0], "python3");
    strcpy(args[1], file_path.c_str());
    args[2] = NULL;
    /******************************************/
    /******************************************/
    /******************************************/
    
    this->create_env();
    pid_t pid;
    int fd = open(".cgi.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        std::cout << "FAIL TO OPEN\n";
        // status = 500 // internal error
    }
    pid = fork();
    if (pid == -1) {
        std::cout << "ERROR fork" << std::endl;
        return 0;
    }
    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execve("/usr/bin/python3", args, _cenv) == -1) {
            std::cout << strerror(errno) << std::endl;
            std::cout << "ERROR execve" << std::endl;
            free(args[0]);
            free(args[1]);
            free(args);
            freeEnv();
            return 0;
        }
    } else {
        free(args[0]);
        free(args[1]);
        free(args);
        freeEnv();
        close(fd);
    }
    waitpid(pid, NULL, 0);
    return 1;
}

bool Cgi::cgiExtension(std::string file_path, Server server, std::string extension)
{   
    (void)server;
    std::string ext = file_path.substr(file_path.rfind(".") + 1, file_path.length() - file_path.rfind(".") + 1);
    // std::cout << "|" << ext << "|" << std::endl;    
    // std::vector<std::string>::iterator it = server.cgi_extension.begin();
    // while (it != server.cgi_extension.end()) {
    //     if (ext == *it) {
    //         return true;
    //     }
    //     ++it;
    // }
    if (file_path.length() >= extension.length()) {
		return (file_path.compare(file_path.length() - extension.length() , extension.length(), extension) == 0);
	}
	return false;
}

bool authorizedMethod(std::string method, Server server) {
    std::vector<std::string>::iterator it = server.method.begin();
    while (it != server.method.end()) {
        if (method == *it) {
            std::cout << "Methode " << *it << " autorisee" << std::endl;
            return true;
        }
        ++it;
    }
    return false;
}

std::string Cgi::displayPage(std::string method, std::map<int, User> Users, int i, Server server)
{
    (void)server;
    // std::cout << "TEST = " << server.getPort() << std::endl;
	status = 200;
	message = "OK";
    std::string file_path = Users[i].getPath().c_str();
	std::stringstream body;
	std::stringstream content;
	std::string data;

	// verifier la taille de la requete :
	if (Users[i].request.size() <= 10000) { // config
        if (authorizedMethod(method, server))
			if (method == "GET" || method == "POST") {
				if (cgiExtension(file_path, server, ".php") || cgiExtension(file_path, server, ".py")) {
					this->execCGI(file_path);
					std::ifstream file(".cgi.txt");
					body << file.rdbuf();
                    if (remove(".cgi.txt") != 0)
                        status = 500;
				} else {
					std::ifstream file(file_path.c_str());
					if (file.fail()) {
						status = 404;
						message = "Not Found";
					} else {
						body << file.rdbuf();
					}
				}
			} else if (method == "DELETE") {
				std::cout << "Methode DELETE" << std::endl;
			} else {
				std::cout << "Methode not implemented" << std::endl;
				status = 501;
				message = "Not Implemented";
			}
        else {
            status = 405;
            message = "Method Not Allowed";
        }
			if (status == 200 && body) {
				data = body.str();
				clength = data.length();
				std::string file_extension = file_path.substr(file_path.find_last_of(".") + 1);

				ctype = "text/html";
				file_extension == "css" ? ctype = "text/css" : ctype = "text/html";
				content << "HTTP/1.1 " << status << " " << message << std::endl;
				content << "Content-Type: " << ctype << std::endl;
				content << "Content-Length: " << clength << std::endl << std::endl;
				content << data;

				return (content.str());
			}
	} else {
		status = 413;
		message = "Request Entity Too Large";
	}
	std::cout << "ERROR STATUS = " << status << std::endl;
	std::ifstream file_error("./pages/error_pages/error_page_505.html");
	body << file_error.rdbuf();
	data = body.str();
	clength = data.length();
	content << "HTTP/1.1 " << status << " " << message << std::endl;
	content << "Content-Type: text/html" << std::endl;
	content << "Content-Length: " << clength << std::endl << std::endl;
	content << data;
	return (content.str());
}