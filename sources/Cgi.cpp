#include "Cgi.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

Cgi::Cgi(void) {}

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

int Cgi::create_env(Request request) {

    _env["SERVER_NAME"] = request.getServer().getServerName(); // conf
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = request.getMethod();
    // _env["REQUEST_METHOD"] = "post";
    _env["CONTENT_TYPE"] = request.getContentType(); // user
    _env["CONTENT_LENGTH"] = request.getContentLength();

    if (request.getMethod() == "GET") {
        std::size_t pos = request.getUri().find('?');
        if (pos != std::string::npos) {
            _env["QUERY_STRING"] = request.getUri().substr(pos + 1, request.getUri().size() - pos - 1);
            _filePath = _filePath.substr(0, _filePath.find('?'));
        }
    } else if (request.getMethod() == "POST") {
        // std::cout << "POSSSSSSSSSSST" << std::endl;
        _env["QUERY_STRING"] = extractQuery(request);
    } else
        _env["QUERY_STRING"] = "";
    _env["SCRIPT_FILENAME"] = _filePath;
    // _env["SCRIPT_FILENAME"] = "/upload.php";
    // _env["QUERY_STRING"] = decodeQuery(_env["QUERY_STRING"]);
    _env["REQUEST_URI"] = request.getUri();
    // _env["REQUEST_URI"] = "/upload.php";

    std::cout << "CONTENT_TYPE : " << _env["CONTENT_TYPE"] << std::endl;
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
        signal(SIGALRM, handleAlarm);
        alarm(3);
        dup2(*fd_in, STDIN_FILENO);
        close(*fd_in);
        dup2(_cgiFd, STDOUT_FILENO);
        close(_cgiFd);
        if (execve(exec, args, _cenv) == -1) {
            close(*fd_out);
            free(args[0]);
            free(args[1]);
            free(args);
            free(exec);
            freeEnv();
            close(_cgiFd);
            std::exit(500);
        }
    }
    close(*fd_out);
    return pid;
}
 // < pipe php ./upload.php > .cgi.txt
int Cgi::execCGI(Request request) {

    // create the execution environment
    if (create_env(request) || _cgiFd == -1)
        return 500;
    
    // executables et arguments (en fonction du fichier de config)
    args = (char **)malloc(sizeof(char *) * 3);
    args[1] = (char *)malloc(sizeof(char) * (_filePath.size() + 1));
    strcpy(args[1], _filePath.c_str());
    args[2] = NULL;
    std::string ext = _filePath.substr(_filePath.rfind(".") + 1);
    if (ext == "py") {
        args[0] = (char *)malloc(sizeof(char) * (strlen("python3") + 1));
        strcpy(args[0], "python3");
        exec = (char *)malloc(sizeof(char) * (strlen("/usr/bin/python3") + 1));
        strcpy(exec, "/usr/bin/python3");
    }
    else if (ext == "php") {
        args[0] = (char *)malloc(sizeof(char) * (strlen("php") + 1));
        strcpy(args[0], "php");
        exec = (char *)malloc(sizeof(char) * (strlen("/usr/bin/php") + 1));
        strcpy(exec, "/usr/bin/php");
    } else {
        return 501;
    }

    std::string body;
    std::string req = request.getAllRequest();
    size_t header_end = req.find("\r\n\r\n");
    if (header_end != std::string::npos)
        body = req.substr(header_end + 4, req.size() - header_end - 4);
    else
        body = "";
    // std::cout << "***********body***********" << std::endl;
    // std::cout << body << std::endl;
    // std::cout << "**************************" << std::endl;
    int fd[2];
    if (pipe(fd) == -1)
        return 500;
    int scriptStatus;
    if (write(fd[1], request.getAllRequest().c_str(), body.size()) == -1) {
        close(fd[0]);
        close(fd[1]);
        return 500;
    }
    waitpid(execScript(&fd[0], &fd[1]), &scriptStatus, 0);
    if (WEXITSTATUS(scriptStatus) == 500)
        return 500;

    free(args[0]);
    free(args[1]);
    free(args);
    free(exec);
    freeEnv();
    close(_cgiFd);
    close(fd[1]);
    close(fd[0]);
    // int st;
    // pid_t child_pid = waitpid(pid, &st, 0);
    // if (child_pid > 0) {
    //     if (WIFSIGNALED(st))
    //         return 408;
    // } else
    //     return 500;
    return 200;
}

void Cgi::handleAlarm(int signal) {
    (void)signal;
    std::cout << "fct handle alarm called !!" << std::endl;
    exit(1);
}
