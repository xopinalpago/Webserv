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
    pid = fork();
    if (pid == -1) {
        std::cout << "ERROR fork" << std::endl;
        return 0;
    }
    int fd = open(".cgi.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0) {
        std::cout << "FAIL TO OPEN\n";
    }
    if (pid == 0) {
        // pour l'instant la requete est dans une string : User[i].request
        // rediriger l'entree standart vers le fichier dans lequel il y aurait la requete ?
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
    }
    waitpid(pid, NULL, 0);
    return 1;
}