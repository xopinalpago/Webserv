#include "Response.hpp"

Response::Response() {}

Response::Response(Request request) {

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

    if (this != &rhs) {
        _request = rhs._request;
        _server = rhs._server;
        messages = rhs.messages;
        backup = rhs.backup;
        types = rhs.types;
        _status = rhs._status;
        _clength = rhs._clength;
        _filePath = rhs._filePath;
        _ctype = rhs._ctype;
        _finalRes = rhs._finalRes;
    }
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

	backup[204] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>204 - No Content</title></head><body><h1>204 - No Content</h1><p id=\"comment\">Oops! Your request has been processed successfully but there is no information to return.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[400] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>400 - Bad Request</title></head><body><h1>400 - Bad Request</h1><p id=\"comment\">Oops! The request syntax is wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[401] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>401 - Unauthorized</title></head><body><h1>401 - Unauthorized</h1><p id=\"comment\">Oops! An authentication is required.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[403] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>403 - Forbidden</title></head><body><h1>403 - Forbidden</h1><p id=\"comment\">Oops! Something went wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[404] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>404 - Page Not Found</title></head><body><h1>404 - Page Not Found</h1><p id=\"comment\">Oops! The page you're looking for does not exist.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[405] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>405 - Method Not Allowed</title></head><body><h1>405 - Method Not Allowed</h1><p id=\"comment\">Oops! The method used is not allowed.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[408] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>408 - Request Time Out</title></head><body><h1>408 - Request Time Out</h1><p id=\"comment\">Oops! The request timed out.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[413] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>413 - Request Entity To Large</title></head><body><h1>413 - Request Entity To Large</h1><p id=\"comment\">Oops! The request is too big.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[500] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>500 - Internal Server Error</title></head><body><h1>500 - Internal Server Error</h1><p id=\"comment\">Oops! Something went wrong.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[501] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>501 - Not Implemented</title></head><body><h1>501 - Not Implemented</h1><p id=\"comment\">Oops! Functionality not implemented by the server.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
	backup[505] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>505 - HTTP Version not supported</title></head><body><h1>505 - HTTP Version not supported</h1><p id=\"comment\">Oops! The HTTP version used is not managed by the server.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
    backup[2000] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>Ressource deleted</title></head><body><h1>Ressource deleted</h1><p id=\"comment\">The ressource was successfully deleted.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
    backup[4040] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/error_page.css\" rel=\"stylesheet\"><title>Ressource not deleted</title></head><body><h1>Ressource not deleted</h1><p id=\"comment\">The ressource you are trying to delete does not exist.</p><p><a href=\"site_index.html\"><button>Index</button></a></p></body></html>";
    backup[1] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"../style/style.css\" rel=\"stylesheet\"><link href=\"../style/upload.css\" rel=\"stylesheet\"><title>Upload</title></head><body><h1 class=\"title\">Upload</h1><p>File successfullyyyyyyy uploaded</p><a href=\"site_index.html\"><button>Index</button></a></body></html>";
    backup[2] = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/upload.css\" rel=\"stylesheet\"><title>Upload</title></head><body><h1 class=\"title\">Upload</h1><p>Could not upload the file</p><a href=\"site_index.html\"><button>Index</button></a></body></html>";
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
    for (size_t i = 0; i < _server.getLoci("/cgi-bin").getCgiEx().size(); ++i) {        
        if (_server.getLoci("/cgi-bin").getCgiEx()[i] == ext) {
            return true;
        }
    }
    return false;
}

void Response::setPathFile()
{
    std::string str = _request.getUri();
	if (str[str.length() - 1] == '/' && !(_request.getContentType() == "multipart/form-data"))
	{
        str = _request.getLocation().getRoot() + "/" + _request.getLocation().getIndex();
    } else {

        std::string uri = _request.getUri();
        if (uri.find('?') != std::string::npos) {
            str = uri.substr(0, uri.find('?'));
        } else {
            str = uri;
        }

        std::string str2 = "/cgi-bin";
        if (IsCgiExtension(str) == true && !(_request.getContentType() == "multipart/form-data")) {
            str = str.substr(1, str.length() - 1);
        } 
        else if (str.compare(0, str2.length(), str2) == 0)
        {
            str = str.substr(str2.size(), str.size() - str2.size());
            str = _request.getLocation().getRoot() + str;
        }
        else if (_request.getLocation().getRoot() == _server.getRoot())
        {
            str = _server.getRoot() + str;
        }
        else
        {
            std::string tempPath2 = str.substr(_request.getLocation().getPath().length(), str.length() - _request.getLocation().getPath().length());
            str = _request.getLocation().getRoot() + tempPath2;
        }
    }
    _filePath = str;
    // std::cout << "_filePath : " << _filePath << std::endl;

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
        _body << backup[_status];
    } else if (errorFile.is_open() == true) {
        errorFile.seekg(0, std::ios::beg);
        _body << errorFile.rdbuf();
    }
}

bool Response::authorizedMethod() {

    for (size_t i = 0; i < _request.getLocation().getMethod().size(); ++i) {
        if (_request.getLocation().getMethod()[i] == _request.getMethod()) {
			// std::cout << _request.getLocation().getPath() << std::endl;
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
    // std::cout << "************HEADER************" << std::endl;
    // std::cout << header.str() << std::endl;
    // std::cout << "******************************" << std::endl;
    return (header.str());
}

void Response::processRequest() {

    if (_request.getContentLength() <= _server.getClientMax()) {
        if (_request.getVersion() != "HTTP/1.1")
            _status = 505;
        else if (authorizedMethod()) {
            if (_request.getMethod() == "GET" || _request.getMethod() == "POST") {
                if (IsCgiExtension(_filePath)) {
                    Cgi *cgi = new Cgi(_filePath);
                    _status = cgi->execCGI(_request);
                    if (_status == 200) {
                        std::ifstream file(".cgi.txt");
                        if (!file.is_open())
                            _status = 500;
                        else if (file.peek() == std::ifstream::traits_type::eof()) {
                            _status = 204;
                        }
                        else {
                            _body << file.rdbuf();
                        }
                    }
                    delete cgi;
                } else {
                    if (_request.getContentType() == "multipart/form-data") {
                        Upload *upload = new Upload(_request);
                        _status = upload->doUpload();
                        if (_status == 1 || _status == 2) {
                            _body << backup[_status];
                            _status = 200;
                        } else
                            _status = 500;
                        delete upload;
                    }
                    else if (_filePath.find('.') != std::string::npos) { // fichier
                        std::ifstream file(_filePath.c_str());
					    if (file.fail()) {
					    	_status = 404;
					    } else
					    	_body << file.rdbuf();
                    }
                    // } else { // dossier
                    //     
                        // directory listing ?
                    // }
                }
            } else if (_request.getMethod() == "DELETE") {
                if (remove(_filePath.c_str()) != 0) {
                    _body << backup[4040];
                } else {
                    _body << backup[2000];
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
            return;
        }
    } else
        _status = 413;
    errorData();
    _content << makeHeader();
	_content << _body.str();
	_finalRes = _content.str();

}
