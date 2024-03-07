#include "Cgi.hpp"

Cgi::Cgi(void) {
    setMessages();
    setBackupPages();
    _status = 200;
    _ctype = "text/html";
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
    errorBackup[2000] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>Ressource deleted</title></head><body><h1>Ressource deleted</h1><p id=\"comment\">The ressource was successfully deleted.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
    errorBackup[4040] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>Ressource not deleted</title></head><body><h1>Ressource not deleted</h1><p id=\"comment\">The ressource you are trying to delete does not exist.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
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

    int len = user.getRequest().allRequest.length() - (user.getRequest().allRequest.rfind("\r\n\r\n") + 4);
    std::string res = user.getRequest().allRequest.substr(user.getRequest().allRequest.rfind("\r\n\r\n") + 4, len);
    return res;
}

int Cgi::create_env(User user) {

    _env["SERVER_NAME"] = "Localhost"; // conf
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = user.getRequest().getMethod();
    _env["CONTENT_TYPE"] = "text/html"; // user
    _env["CONTENT_LENGTH"] = "882"; // user

    if (user.getRequest().getMethod() == "GET") {
        std::size_t pos = user.getRequest().getUri().find('?');
        if (pos != std::string::npos) {
            _env["QUERY_STRING"] = user.getRequest().getUri().substr(pos + 1, user.getRequest().getUri().size() - pos - 1);
            _filePath = _filePath.substr(0, _filePath.find('?'));
        }
    } else if (user.getRequest().getMethod() == "POST") {
        _env["QUERY_STRING"] = extractQuery(user);
    } else
        _env["QUERY_STRING"] = "";
    _env["SCRIPT_FILENAME"] = _filePath;
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

bool Cgi::scriptExt(std::string ext, std::string method) {

	if (method == "GET")
		_filePath = _filePath.substr(0, _filePath.find('?'));
	if (_filePath.length() >= ext.length()) {
		return (_filePath.compare(_filePath.length() - ext.length() , ext.length(), ext) == 0);
	}
	return false;
}


int Cgi::execCGI(User user) {
    
    // creer l'environnement d'execution
    if (this->create_env(user))
        return 500;
    
    char **args;
    args = (char **)malloc(sizeof(char *) * 3);
    args[1] = (char *)malloc(sizeof(char) * 8);
    strcpy(args[1], _filePath.c_str());
    args[2] = NULL;

    // determiner l'executable
    char *exec;
    
    if (scriptExt("py", user.getRequest().getMethod()) == true) {
        args[0] = (char *)malloc(sizeof(char) * (strlen("python3") + 1));
        strcpy(args[0], "python3");
        exec = (char *)malloc(sizeof(char) * (strlen("/usr/bin/python3") + 1));
        strcpy(exec, "/usr/bin/python3");
    }
    else if (scriptExt("php", user.getRequest().getMethod()) == true) { // config
        args[0] = (char *)malloc(sizeof(char) * (strlen("php") + 1));
        strcpy(args[0], "php");
        exec = (char *)malloc(sizeof(char) * (strlen("/usr/bin/php") + 1));
        strcpy(exec, "/usr/bin/php");
    } else {
        return 501;
    }
    
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
        dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execve(exec, args, _cenv) == -1) {
            free(args[0]);
            free(args[1]);
            free(args);
            free(exec);
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
    
    // Server server = user.getRequest().getServer();
    // std::vector<std::string>::iterator it = server.getMethod().begin();
    // std::vector<std::string>::iterator ite = server.getMethod().end();
    // while (it != ite) {
    //     if (user.getRequest().getMethod() == *it)
    //         return true;
    //     ++it;
    // }

    // return false;

    for (size_t i = 0; i < user.getRequest().getServer().getMethod().size(); ++i) {
        // Vérifier si l'élément correspond
        if (user.getRequest().getServer().getMethod()[i] == user.getRequest().getMethod()) {
            return true; // Element trouvé
        }
    }
    return false; // Element non trouvé
}

bool Cgi::IsCgiExtension(User user)
{   
    std::string ext = _filePath.substr(_filePath.rfind(".") + 1, _filePath.length() - _filePath.rfind(".") + 1);
    // Server server = user.getServer();

    // std::vector<std::string>::iterator it = user.getRequest().getServer().getCgiEx().begin(); 
    // std::vector<std::string>::iterator ite = user.getRequest().getServer().getCgiEx().end();
    // while (it != ite) {
    //     if (ext == *it)
    //         return true;
    //     ++it;
    // }
    // return false;

    for (size_t i = 0; i < user.getRequest().getServer().getCgiEx().size(); ++i) {
        // Vérifier si l'élément correspond
        if (user.getRequest().getServer().getCgiEx()[i] == ext) {
            return true; // Element trouvé
        }
    }
    return false; // Element non trouvé
}

std::string Cgi::setPathFile(std::string str, User &user)
{
	if (!str.compare("/"))
	{
		// std::cout << "user.getRequest().getServer().getIndex()= " << user.getRequest().getServer().getIndex() << std::endl;
		// std::cout << server.getServerName() << std::endl; 
		str = user.getRequest().getServer().getIndex();
	}
	else if (IsCgiExtension(user) || IsCgiExtension(user)) // definir en fct du fichier de config
		return str.substr(1, str.length() - 1);
	else
		str = "pages" + str;
    return (str);
}

std::string Cgi::displayPage(std::string method, User &user)
{
    _filePath = setPathFile(user.getRequest().getUri(), user).c_str();
	// verifier la taille de la requete :
	if (user.getRequest().getContentLength() <= user.getRequest().getServer().getClientMax()) { // config
        if (authorizedMethod(user))
			if (method == "GET" || method == "POST") {
				if (IsCgiExtension(user)) {
					_status = this->execCGI(user);
                    if (_status == 200) {
					    std::ifstream file(".cgi.txt");
					    _body << file.rdbuf();
                    }
				} else {
					std::ifstream file(_filePath.c_str());
					if (file.fail()) {
						_status = 404;
					} else
						_body << file.rdbuf();
				}
			} else if (method == "DELETE") {
                if (remove(_filePath.c_str()) != 0) {
                    _body << errorBackup[4040];
                } else {
                    _body << errorBackup[2000];
                }
			} else
				_status = 501;
        else
            _status = 405;
		if (_status == 200 && _body) {
			_data = _body.str();
			std::string file_extension = _filePath.substr(_filePath.find_last_of(".") + 1);
			file_extension == "css" ? _ctype = "text/css" : _ctype = "text/html";
            _content << makeHeader();
			_content << _data;
			return (_content.str());
		}
	} else
		_status = 413;
    errorData(user);
    _content << makeHeader();
	_content << _data;
	return (_content.str());
}

std::string Cgi::makeHeader() {

    std::stringstream header;
    _clength = _data.length();
    header << "HTTP/1.1 " << _status << " " << messages[_status] << std::endl;
    header << "Content-Type: " << _ctype << std::endl;
    header << "Content-Length: " << _clength << std::endl << std::endl;
    return (header.str());
}

void Cgi::errorData(User user) {

    std::ifstream errorFile;
    Server server = user.getRequest().getServer();
    std::map<int, std::string>::iterator it = user.getRequest().getServer().getErrorPage().find(_status);
    // std::map<int, std::string>::iterator ite = server.getErrorPage().end();
    // if (it != ite) {
    if (it != user.getRequest().getServer().getErrorPage().end()) {
        errorFile.open(((server.getErrorPage())[_status]).c_str());
    } else {
        std::stringstream ss;
        ss << "./pages/error_pages/error_page_" << _status << ".html";
        std::string fileName = ss.str();
	    errorFile.open(fileName.c_str());
    }
    if (!errorFile.is_open()) {
        _data = errorBackup[_status];
    } else {
	    _body << errorFile.rdbuf();
	    _data = _body.str();
    }
}