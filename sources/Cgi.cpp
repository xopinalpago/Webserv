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

    int len = request.allRequest.length() - (request.allRequest.rfind("\r\n\r\n") + 4);
    std::string res = request.allRequest.substr(request.allRequest.rfind("\r\n\r\n") + 4, len);
    return res;
}

int Cgi::create_env(Request request) {

    _env["SERVER_NAME"] = "Localhost"; // conf
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REQUEST_METHOD"] = request.getMethod();
    _env["CONTENT_TYPE"] = "text/html"; // user
    _env["CONTENT_LENGTH"] = "882"; // user

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
    return (mapToChar());
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

    
int Cgi::execCGI(Request request) {
    char **args = NULL;
    char *exec = NULL;

    // create the execution environment
    if (create_env(request) || _cgiFd == -1)
        return 500;
    
    args = (char **)malloc(sizeof(char *) * 3);
    args[1] = (char *)malloc(sizeof(char) * (_filePath.size() + 1));
    strcpy(args[1], _filePath.c_str());
    args[2] = NULL;

    // determine executable and arguments
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


    // int pipefd[2];
    // if (pipe(pipefd) == -1) {
    //     return 501;
    // }
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        return 500;
    }
    if (pid == 0) {
        signal(SIGALRM, handleAlarm);
        alarm(3);
        // close(pipefd[0]);
        // dup2(pipefd[1], STDOUT_FILENO);
        // close(pipefd[1]);
        dup2(_cgiFd, STDOUT_FILENO);
        close(_cgiFd);
        if (execve(exec, args, _cenv) == -1) {
            free(args[0]);
            free(args[1]);
            free(args);
            free(exec);
            freeEnv();
            close(_cgiFd);
            return 500;
        }
    } else {
        free(args[0]);
        free(args[1]);
        free(args);
        free(exec);
        freeEnv();
        close(_cgiFd);

        std::cout << "LA" << std::endl;
        int st;
        // Attendre la fin du processus enfant
        pid_t child_pid = waitpid(pid, &st, 0);
        if (child_pid > 0) {
            // Vérifier si le processus enfant s'est terminé normalement
            if (WIFEXITED(st)) {
                std::cout << "Le processus enfant s'est terminé avec le code de sortie : " << WEXITSTATUS(st) << std::endl;
            } else if (WIFSIGNALED(st)) {
                return 408;
                std::cout << "Le processus enfant s'est terminé à cause du signal : " << WTERMSIG(st) << std::endl;
            }
        } else {
            std::cerr << "Erreur lors de la récupération du statut du processus enfant" << std::endl;
        }
    }
    // waitpid(pid, NULL, 0);
    return 200;
}

void Cgi::handleAlarm(int signal) {
    (void)signal;
    std::cout << "fct handle alarm called !!" << std::endl;
    exit(1);
}
