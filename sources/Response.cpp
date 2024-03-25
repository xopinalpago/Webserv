#include "Response.hpp"

Response::Response() {}

// Response::Response(Request request, Launcher* launch) {

//     _request = request;
//     _server = request.getServer();
//     _launch = launch;
//     setMessages();
//     setBackupPages();
//     setTypes();
//     _status = 200;
//     _ctype = "text/html";
//     setPathFile();
//     processRequest();
// }

Response::Response(Request request, s_socketInfo* infos) {

    _infos = infos;
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
    messages[300] = "Multiple Choices";
    messages[301] = "Moved Permanently";
    messages[302] = "Found";
    messages[303] = "See Other";
    messages[304] = "Not Modified";
    messages[305] = "Use Proxy";
    messages[307] = "Temporary Redirect";
    messages[308] = "Permanent Redirect";
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

bool Response::isDirectory(std::string filePath) {

    std::string path2;
    if (filePath[filePath.size() - 1] == '/')
        path2 = filePath.substr(0, filePath.size() - 1);
    else
        path2 = filePath;
    if (path2[0] == '/')
        path2 = path2.substr(1, path2.size() - 1);
    struct stat path_stat;
    if (stat(path2.c_str(), &path_stat) == 0) {
        if (S_ISDIR(path_stat.st_mode) != 0) {
            return true;
        }
    }
    return false;
}

void Response::setPathFile()
{
    std::string str = _request.getUri();
    // std::cout << std::endl;
    // std::cout << "_request.getUri() : " << _request.getUri() << std::endl;
    // std::cout << "_request.getLocation().getPath() : " << _request.getLocation().getPath() << std::endl;

    if (_request.getLocation().getRedirectionPath().size() > 0) {
		return ;
	}
    if (str == "/") {
        if (_request.getLocation().getIndex().size() > 1)   {
            for (size_t i = 0; i < _request.getLocation().getIndex().size(); i++)   {
                std::string tmp = _request.getLocation().getRoot() + "/" + _request.getLocation().getIndexi(i);
                if (access(str.c_str(), R_OK) == 0) {
                    str = tmp; 
                    break;
                }
            }
        }
        else {
            str = _request.getLocation().getRoot() + "/";
        }

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
        else if (str.compare(0, str2.length(), str2) == 0)  {
            str = str.substr(str2.size(), str.size() - str2.size());
            str = _request.getLocation().getRoot() + str;
        }
        else if (_request.getLocation().getRoot() == _server.getRoot()) {
            str = _server.getRoot() + str;
            std::cout << "TEST1" << std::endl;
        }
        else    {
            std::string tempPath2;
            if (_request.getLocation().getPath() == "/")    {
                tempPath2 = str;
            }
            else {
                tempPath2 = str.substr(_request.getLocation().getPath().length(), str.length() - _request.getLocation().getPath().length());
            }
            if (_request.getLocation().getRoot()[_request.getLocation().getRoot().size() - 1] == '/' && tempPath2[0] == '/') {
                str = _request.getLocation().getRoot() + tempPath2.substr(1, tempPath2.size() - 1);
            }
            else
                str = _request.getLocation().getRoot() + tempPath2;
        }
    }
    _filePath = str;
    for (size_t i = 0; i < _request.getLocation().getIndex().size(); i++)   {
        if (isDirectory(_filePath) && _request.getLocation().getIndexi(i) != "") {
            std::cout << "CHAR = " << std::endl;
            std::string tmp = "";
                tmp = _request.getLocation().getRoot() + "/" + _request.getLocation().getIndexi(i);
            if (access(tmp.c_str(), R_OK) == 0) {
                _filePath = tmp;
                break;
            }
        }
    }
    std::cout << "_filePath : " << _filePath << std::endl;
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
    // std::cout << "_status : " << _status << std::endl;
    if (_status != 200)
        _ctype = types["html"];
    else
        _ctype = types[ext];
    if (_ctype == "")
        _ctype = types["html"];
    header << "HTTP/1.1 " << _status << " " << messages[_status] << std::endl;
    header << "Content-Type: " << _ctype << std::endl;
    header << "Content-Length: " << _clength << std::endl << std::endl;
    return (header.str());
}

std::string Response::getListJson() {

    std::vector<std::string> lstFiles;
    DIR *dir;
    struct dirent *ent;

    if (_filePath[0] == '/')
        _filePath = _filePath.substr(1, _filePath.size() - 1);
    dir = opendir(_filePath.c_str());
    if (dir != NULL) {
        std::cout << "_filePath : " << _filePath << std::endl;
        while ((ent = readdir(dir)) != NULL) {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                lstFiles.push_back(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        std::cout << "ERROR" << std::endl;
    }
    std::sort(lstFiles.begin(), lstFiles.end());
    if (lstFiles.empty())
        return "";
    std::stringstream json;
    json << "[";
    for (size_t i = 0; i < lstFiles.size(); i++) {
        json << "\"" << lstFiles[i] << "\"";
        if (i < lstFiles.size() - 1)
            json << ", ";
    }
    json << "]";
    std::cout << "json ; " << json.str() << std::endl;
    return json.str();
}

void Response::processRequest() {

    // std::cout << "taille : " << _request.getAllRequest().size() << std::endl;
    // std::cout << "UPLOAD" << std::endl;
    std::cout << "LOCATION = " << _request.getLocation().getPath() << std::endl;
    // std::cout << "_filePath : " << _filePath << std::endl;
    if (_request.getAllRequest().size() <= _server.getClientMax()) {
        if (_request.getVersion() != "HTTP/1.1")
            _status = 505;
        else if (_request.getLocation().getRedirectionPath() != "")
        {
            _status = _request.getLocation().getRedirectionCode();
            _content << "HTTP/1.1 " << _status << " " << messages[_status] << std::endl;
            _content << "Location: " << _request.getLocation().getRedirectionPath() << std::endl;
			_content << "Content-Length: " << 0 << std::endl << std::endl;
            _finalRes = _content.str();
            return;
        }
        else if (authorizedMethod()) {
            if (_request.getMethod() == "GET" || _request.getMethod() == "POST") {
                if (_request.getLocation().getPath() == "/listFiles") {
                    _body << getListJson();
                }
                else if (IsCgiExtension(_filePath)) {
                    Cgi *cgi = new Cgi(_filePath, _infos);
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
                        std::cout << "UPLOAD" << std::endl;
                        Upload *upload = new Upload(_request);
                        _status = upload->doUpload();
                        if (_status == 1) {
                            _body << backup[_status];
                            _status = 200;
                        } else if (_status == 2)
                            _status = 400;
                        else
                            _status = 500;
                        delete upload;
                    }
                    else if (isDirectory(_filePath)) {
                        std::cout << "DIRECTORY" << std::endl;
						if (access(_filePath.c_str(), R_OK) == -1) {
							_status = 403;
						}
                        else if (_request.getLocation().getAutoindex() == 1) {
                            _status = directoryListing(_filePath);
                        } else
                            _status = 403;
                    }
                    else {
                        // std::cout << "FILE" << std::endl;
						struct stat fileStat;
						if (stat(_filePath.c_str(), &fileStat) != 0) {
							_status = 404;
						}
                        else if (access(_filePath.c_str(), R_OK) == -1) {
                        	std::cout << "access" << std::endl;
                            _status = 403;
                        }
						else	{
							std::ifstream file(_filePath.c_str());
							if (file.fail()) {
								_status = 404;
							} else
								_body << file.rdbuf();
						}
                    }
                }
            } else if (_request.getMethod() == "DELETE") {
                std::cout << "LAAAAAAA" << std::endl;
                std::cout << "_filePath : " << _filePath << std::endl;
                if (_filePath[0] == '/')
                    _filePath = _filePath.substr(1, _filePath.size() - 1);
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
            _ctype = types[_filePath.substr(_filePath.rfind(".") + 1)];
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
	// std::cout << "************FINAL RES************" << std::endl;
	// std::cout << _finalRes << std::endl;
	// std::cout << "******************************" << std::endl;
}

int Response::directoryListing(const std::string& directoryPath) {

    std::vector<std::string> lstFiles;
    DIR *dir;
    struct dirent *ent;

    dir = opendir(directoryPath.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                std::string name(ent->d_name);
            	if (ent->d_type == DT_DIR)
			        name += "/";
                lstFiles.push_back(name);
            }
        }
        closedir(dir);
    } else {
        return 500;
    }
    std::sort(lstFiles.begin(), lstFiles.end());
    std::string response = "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of /</title>\n</head>\n<body>\n";
    response += "<h1>Index of /</h1>\n<hr>\n<ul>\n";
    for (std::vector<std::string>::const_iterator it = lstFiles.begin(); it != lstFiles.end(); ++it) {
        std::cout << "NAME = " << *it << std::endl;
        response += "<li><a href=\"" + *it + "\">" + *it + "</a></li>\n";
    }
    response += "</ul>\n<hr>\n</body>\n</html>\n";
    _body << response;
    return 200;
}