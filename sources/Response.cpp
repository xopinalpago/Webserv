#include "Response.hpp"

Response::Response() {

    setMessages();
    setBackupPages();
    _status = 200;
    _ctype = "text/html";
}

Response::Response(Request request) {

    // _request = user.getRequest();
    _request = request;
    _server = request.getServer();
    setMessages();
    setBackupPages();
    setTypes();
    _status = 200;
    _ctype = "text/html";
    setPathFile();
    processRequest();
}

Response::~Response() {}

Response::Response(const Response& cpy) {
    *this = cpy;
}

Response& Response::operator=(const Response& rhs) {
    (void)rhs;
    // if (this != &rhs) {

    // }
    return *this;
}

void Response::setMessages() {

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

void Response::setBackupPages() {

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

void Response::setTypes() {
    types["html"] = "text/html";
    types["txt"] = "text/plain";
    types["json"] = "application/json";
    types["xml"] = "application/xml";
    types["jpeg"] = "image/jpeg";
    types["png"] = "image/png";
    types["gif"] = "image/gif";
    types["pdf"] = "application/pdf";
    types["js"] = "application/javascript";
    types["css"] = "text/css";
    types["mp3"] = "audio/mpeg";
    types["mp4"] = "video/mp4";
    types["zip"] = "application/zip";
}

bool Response::IsCgiExtension(std::string file)
{   
    if (file == "")
        return false;
    
    size_t pos = file.rfind(".");
    std::string ext = file.substr(pos + 1, _filePath.length() - pos + 1);
    for (size_t i = 0; i < _server.getCgiEx().size(); ++i) {
        if (_server.getCgiEx()[i] == ext) {
            return true;
        }
    }
    return false;
}

void Response::setPathFile()
{
    std::string str = _request.getUri();
	if (!str.compare("/"))
	{
		str = _server.getIndex();
	} else {
        std::string uri = _request.getUri();
        if (uri.find('?') != std::string::npos) {
            str = uri.substr(0, uri.find('?'));
        } else {
            str = uri;
        }
        if (IsCgiExtension(str) == true) {
            str = str.substr(1, str.length() - 1);
        } else {
            str = "pages" + str;
        }
    }
    _filePath = str;
}


void Response::errorData() {

    std::ifstream errorFile;
    std::map<int, std::string>::iterator it = _server.getErrorPage().find(_status);
    std::map<int, std::string>::iterator ite = _server.getErrorPage().end();
    if (it != ite) {
        errorFile.open((_server.getErrorPage()[_status]).c_str());
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

bool Response::authorizedMethod() {

    for (size_t i = 0; i < _server.getMethod().size(); ++i) {
        if (_server.getMethod()[i] == _request.getMethod()) {
            return true;
        }
    }
    return false;
}

std::string Response::makeHeader() {

    std::stringstream header;
    std::string ext = _filePath.substr(_filePath.rfind(".") + 1);

    _clength = _body.str().length();
    _ctype = types[ext];
    header << "HTTP/1.1 " << _status << " " << messages[_status] << std::endl;
    header << "Content-Type: " << _ctype << std::endl;
    header << "Content-Length: " << _clength << std::endl << std::endl;
    return (header.str());
}

void Response::processRequest() {

    if (_request.getContentLength() <= _server.getClientMax()) {
        if (authorizedMethod()) {
            if (_request.getMethod() == "GET" || _request.getMethod() == "POST") {
                if (IsCgiExtension(_filePath)) {
                    Cgi *cgi = new Cgi(_filePath);
                    _status = cgi->execCGI(_request);
                    if (_status == 200) {
                        std::ifstream file(".cgi.txt");
					    _body << file.rdbuf();
                    }
                    delete cgi;
                } else {
                    std::ifstream file(_filePath.c_str());
					if (file.fail()) {
						_status = 404;
					} else
						_body << file.rdbuf();
                }
            } else if (_request.getMethod() == "DELETE") {
                if (remove(_filePath.c_str()) != 0) {
                    _body << errorBackup[4040];
                } else {
                    _body << errorBackup[2000];
                }
            } else
                _status = 501;
        } else
            _status = 405;
        if (_status == 200 && _body) {
            std::string ext = _filePath.substr(_filePath.rfind(".") + 1);
            _ctype = types[ext];
            _content << makeHeader();
			_content << _body.str();
			_finalRes = _content.str();
        }
    } else
        _status = 413;
    errorData();
    _content << makeHeader();
	_content << _body.str();
	_finalRes = _content.str();
}
