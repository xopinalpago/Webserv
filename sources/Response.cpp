#include "Response.hpp"

Response::Response() {

    setMessages();
    setBackupPages();
    _status = 200;
    _ctype = "text/html";
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

std::string Response::makeHeader() {

    std::stringstream header;
    _clength = _data.length();
    header << "HTTP/1.1 " << _status << " " << messages[_status] << std::endl;
    header << "Content-Type: " << _ctype << std::endl;
    header << "Content-Length: " << _clength << std::endl << std::endl;
    return (header.str());
}

void Response::errorData(User user) {

    std::ifstream errorFile;
    Server server = user.getServer();
    std::map<int, std::string>::iterator it = server.error_page.find(_status);
    std::map<int, std::string>::iterator ite = server.error_page.end();
    if (it != ite) {
        errorFile.open((server.error_page[_status]).c_str());
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