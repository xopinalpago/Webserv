#include "Cgi.hpp"

Cgi::Cgi(void) {
    setMessages();
    setBackupPages();
}

Cgi::~Cgi(void) {}

void Cgi::setMessages() {

	messages[100] = "Continue";
	messages[200] = "OK";
	messages[204] = "No Content";
	messages[400] = "Bad Request";
	messages[401] = "Unauthorized";
	messages[403] = "Forbidden";
	messages[404] = "Not Found";
	messages[405] = "Method Not Allowed";
	messages[408] = "Request Time Out";
	messages[413] = "Request Entity Too Large";
	messages[500] = "Internal Server Error";
	messages[501] = "Not Implemented";
	messages[505] = "HTTP Version not supported";
}

void Cgi::setBackupPages() {

	errorBackup[204] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>204 - No Content</title></head><body><h1>204 - No Content</h1><p id=\"comment\">Oops! Your request has been processed successfully but there is no information to return.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[400] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>400 - Bad Request</title></head><body><h1>400 - Bad Request</h1><p id=\"comment\">Oops! The request syntax is wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[401] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>401 - Unauthorized</title></head><body><h1>401 - Unauthorized</h1><p id=\"comment\">Oops! An authentication is required.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[403] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>403 - Forbidden</title></head><body><h1>403 - Forbidden</h1><p id=\"comment\">Oops! Something went wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[404] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>404 - Page Not Found</title></head><body><h1>404 - Page Not Found</h1><p id=\"comment\">Oops! The page you're looking for does not exist.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[405] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>405 - Method Not Allowed</title></head><body><h1>405 - Method Not Allowed</h1><p id=\"comment\">Oops! The method used is not allowed.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[408] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>408 - Request Time Out</title></head><body><h1>408 - Request Time Out</h1><p id=\"comment\">Oops! The request timed out.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[413] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>413 - Request Entity To Large</title></head><body><h1>413 - Request Entity To Large</h1><p id=\"comment\">Oops! The request is too big.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[500] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>500 - Internal Server Error</title></head><body><h1>500 - Internal Server Error</h1><p id=\"comment\">Oops! Something went wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[501] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>501 - Not Implemented</title></head><body><h1>501 - Not Implemented</h1><p id=\"comment\">Oops! Functionality not implemented by the server.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	errorBackup[505] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>505 - HTTP Version not supported</title></head><body><h1>505 - HTTP Version not supported</h1><p id=\"comment\">Oops! The HTTP version used is not managed by the server.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
}

std::string Cgi::decodeQuery(std::string query) {

    std::stringstream res;
    for (size_t i = 0 ; i < query.length() ; i++) {
        if (query[i] == '+')
            res << ' ';
        else if (query[i] == '%') {
            int value;
            std::string hex = query.substr(i + 1, 2);
            std::stringstream ss;
            ss << std::hex << hex;
            ss >> value;
            res << static_cast<char>(value);
            i += 2;
        } else {
            res << query[i];
        }
    }
    return res.str();
}

std::string Cgi::extractQuery(User user) {

    int len = user.request.length() - (user.request.rfind("\r\n\r\n") + 4);
    std::string res = user.request.substr(user.request.rfind("\r\n\r\n") + 4, len);
    return res;
}

int Cgi::create_env(User user, std::string *file_path) {

    _env["SERVER_NAME"] = "Localhost"; // conf
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = user.getMethod();
    _env["CONTENT_TYPE"] = "text/html"; // user
    _env["CONTENT_LENGTH"] = "882"; // user

    if (user.getMethod() == "GET") {
        std::size_t pos = user.request.find('?');
        if (pos != std::string::npos) {
            _env["QUERY_STRING"] = user.request.substr(pos + 1, user.request.find(' ', pos) - pos - 1);
            *file_path = file_path->substr(0, file_path->find('?'));
        }
    } else if (user.getMethod() == "POST") {
        _env["QUERY_STRING"] = extractQuery(user);
    } else
        _env["QUERY_STRING"] = "";
    _env["SCRIPT_FILENAME"] = *file_path;
    // _env["QUERY_STRING"] = decodeQuery(_env["QUERY_STRING"]);
    _env["REQUEST_URI"] = "uri";
    return (this->mapToChar());
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


int Cgi::execCGI(std::string file_path,User user) {
    
    if (this->create_env(user, &file_path))
        return 500;
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
    
    pid_t pid;
    int fd = open(".cgi.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        std::cout << "FAIL TO OPEN\n";
        return 500;
    }
    pid = fork();
    if (pid == -1) {
        std::cout << "ERROR fork" << std::endl;
        return 500;
    }
    if (pid == 0) {
        // dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execve("/usr/bin/python3", args, _cenv) == -1) {
            std::cout << strerror(errno) << std::endl;
            std::cout << "ERROR execve" << std::endl;
            free(args[0]);
            free(args[1]);
            free(args);
            freeEnv();
            return 500;
        }
    } else {
        free(args[0]);
        free(args[1]);
        free(args);
        freeEnv();
        close(fd);
    }
    waitpid(pid, NULL, 0);
    return 200;
}

bool Cgi::authorizedMethod(User user) {
    
    Server server = user.getServer();
    std::vector<std::string>::iterator it = server.method.begin();
    std::vector<std::string>::iterator ite = server.method.end();
    while (it != ite) {
        if (user.getMethod() == *it)
            return true;
        ++it;
    }
    return false;
}

bool Cgi::cgiExtension(std::string file_path, User user)
{   
    std::string ext = file_path.substr(file_path.rfind(".") + 1, file_path.length() - file_path.rfind(".") + 1);
    Server server = user.getServer();

    std::vector<std::string>::iterator it = server.cgi_extension.begin();
    std::vector<std::string>::iterator ite = server.cgi_extension.end();
    while (it != ite) {
        if (ext == *it)
            return true;
        ++it;
    }
    return false;
}

std::string Cgi::displayPage(std::string method, User &user)
{
	status = 200;
	message = "OK";
    std::string file_path = user.getPath(method).c_str();
	std::stringstream body;
	std::stringstream content;
	std::string data;
	// verifier la taille de la requete :
	if (user.request.size() <= 10000) { // config
        if (authorizedMethod(user))
			if (method == "GET" || method == "POST") {
				if (cgiExtension(file_path, user)) {
					status = this->execCGI(file_path, user);
                    if (status == 200) {
					    std::ifstream file(".cgi.txt");
					    body << file.rdbuf();
                        // if (remove(".cgi.txt") != 0)
                        //     status = 500;
                    }
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
                status = 404;
			} else {
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
    std::ifstream errorFile;
    Server server = user.getServer();
    std::map<int, std::string>::iterator it = server.error_page.find(status);
    std::map<int, std::string>::iterator ite = server.error_page.end();
    if (it != ite) {
        errorFile.open((server.error_page[status]).c_str());
    } else {
        std::stringstream ss;
        ss << "./pages/error_pages/error_page_" << status << ".html";
        std::string fileName = ss.str();
	    errorFile.open(fileName.c_str());
    }
    if (!errorFile.is_open()) {
        data = errorBackup[status];
    } else {
	    body << errorFile.rdbuf();
	    data = body.str();
    }
	clength = data.length();
	content << "HTTP/1.1 " << status << " " << messages[status] << std::endl;
	content << "Content-Type: text/html" << std::endl;
	content << "Content-Length: " << clength << std::endl << std::endl;
	content << data;
	return (content.str());
}
