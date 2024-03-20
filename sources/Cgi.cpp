#include "Cgi.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

Cgi::Cgi(void) {}

Cgi::Cgi(const Cgi& cpy) {
    *this = cpy;
}

Cgi& Cgi::operator=(const Cgi& rhs) {
    if (this != &rhs) {
        _env = rhs._env;
        _cenv = rhs._cenv;
        _cgiFd = rhs._cgiFd;
        _cgiFile = rhs._cgiFile;
        _filePath = rhs._filePath;
        _args = rhs._args;
        exec = rhs.exec;
    }
    return *this;
}

Cgi::Cgi(std::string filePath) {
    
    _filePath = filePath;
    _cgiFile = ".cgi.txt";
    _cgiFd = open(_cgiFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if (_cgiFd != -1) {
        if (chmod(_cgiFile.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            close(_cgiFd);
            _cgiFd = -1;
        }
    }
    exec = NULL;
    _args = NULL;
}

Cgi::~Cgi(void) {

    if (fcntl(_cgiFd, F_GETFL) == -1)
        close(_cgiFd);
    // remove(_cgiFile.c_str());
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

std::string Cgi::extractQuery(Request request) {

    int len = request.getAllRequest().length() - (request.getAllRequest().rfind("\r\n\r\n") + 4);
    std::string res = request.getAllRequest().substr(request.getAllRequest().rfind("\r\n\r\n") + 4, len);
    return res;
}

int Cgi::mapToChar() {

    memset(&_cenv, 0, sizeof(_cenv));
    std::map<std::string, std::string>::iterator it;
    _cenv = new char*[_env.size() + 1];
    if (!_cenv)
        return 1;
    int i = 0;
    for (it = _env.begin() ; it != _env.end() ; ++it) {
        int len = (it->first).length() + 1 + (it->second).length() + 1;
        _cenv[i] = new char [len];
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

int Cgi::create_env(Request request) {

    _env["SERVER_NAME"] = request.getServer().getServerName(); // conf
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = request.getMethod();
    _env["CONTENT_TYPE"] = request.getContentType(); // user
    _env["CONTENT_LENGTH"] = request.getContentLength();

    if (request.getMethod() == "GET") {
        std::size_t pos = request.getUri().find('?');
        if (pos != std::string::npos) {
            _env["QUERY_STRING"] = request.getUri().substr(pos + 1, request.getUri().size() - pos - 1);
            _filePath = _filePath.substr(0, _filePath.find('?'));
        }
    } else if (request.getMethod() == "POST") {
        _env["QUERY_STRING"] = extractQuery(request);
    } else
        _env["QUERY_STRING"] = "";
    _env["SCRIPT_FILENAME"] = _filePath;
    // _env["QUERY_STRING"] = decodeQuery(_env["QUERY_STRING"]);
    _env["REQUEST_URI"] = request.getUri();

    // std::cout << "CONTENT_TYPE : " << _env["CONTENT_TYPE"] << std::endl;
    // std::cout << "SCRIPT_FILENAME : " << _env["SCRIPT_FILENAME"] << std::endl;

    return (mapToChar());
}

int Cgi::execScript(int *fd_in, int *fd_out) {

    pid_t pid;
    pid = fork();
    if (pid == -1) {
        return 500;
    }
    if (pid == 0) {
        alarm(3);
        dup2(*fd_in, STDIN_FILENO);
        close(*fd_in);
        dup2(_cgiFd, STDOUT_FILENO);
        close(_cgiFd);
        if (execve(exec, _args, _cenv) == -1) {
            close(*fd_out);
            free_tabs();
            close(_cgiFd);
            std::exit(500);
        }
    }
    close(*fd_out);
    return pid;
}

void Cgi::free_tabs() {
    int i = 0;
    while (_args[i]) {
        delete [] _args[i];
        i++;
    }
    delete [] _args;

    i = 0;
    while (_cenv[i]) {
        delete [] _cenv[i];
        i++;
    }
    delete [] _cenv;

    delete [] exec;
}

int Cgi::writePipe(int *fd_in, int *fd_out, std::string body) {

    pid_t pid;
    char * info;
    info = new char[body.size() + 1];
    strcpy(info, body.c_str());
    pid = fork();
    if (pid == -1) {
        return 500;
    }
    if (pid == 0) {
        alarm(3);
        dup2(*fd_out, STDOUT_FILENO);
        close(*fd_out);
        close(*fd_in);
        char *const args[] = { (char*)"/usr/bin/echo", info, NULL };
        if (execve("/usr/bin/echo", args, NULL) == -1) {
            free_tabs();
            delete [] info;
            close(*fd_in);
            std::exit(500);
        }
    }
    delete [] info;
    close(*fd_out);
    return pid;
}

void Cgi::findArgs(Request& request) {

    std::map<std::string, std::string> paths = request.getLocation().getCgiPath();
    std::map<std::string, std::string>::iterator it = paths.begin();
    std::map<std::string, std::string>::iterator ite = paths.end();


    std::string ext = _filePath.substr(_filePath.rfind("."));

    _args = new char*[3];
    _args[1] = new char [_filePath.size() + 1];
    strcpy(_args[1], _filePath.c_str());
    _args[2] = NULL;
    while (it != ite) {
        if (ext == it->first) {
            _args[0] = new char [strlen(paths[ext].c_str()) + 1];
            strcpy(_args[0], paths[ext].c_str());
            exec = new char [strlen(paths[ext].c_str()) + 1];
            strcpy(exec, paths[ext].c_str());
        }
        ++it;
    }
}

int Cgi::execCGI(Request request) {

    int scriptStatus;
    int writeStatus;
    int fd[2];

    if (create_env(request) || _cgiFd == -1)
        return 500;
    
    findArgs(request);
    if (!exec)
        return 501;
    
    if (pipe(fd) == -1)
        return 500;
    waitpid(execScript(&fd[0], &fd[1]), &scriptStatus, 0);
    waitpid(writePipe(&fd[0], &fd[1], request.getBody()), &writeStatus, 0);
    if (WIFSIGNALED(scriptStatus))
        return 408;
    if (WEXITSTATUS(scriptStatus) == 500)
        return 500;
    free_tabs();
    close(_cgiFd);
    close(fd[1]);
    close(fd[0]);
    return 200;
}
