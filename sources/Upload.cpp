#include "Upload.hpp"

Upload::Upload() {}

Upload::Upload(Request request) {

    filename = "";
    fileBody = "";
    _request = request;
    _server = request.getServer();
}

Upload::~Upload() {}

Upload::Upload(const Upload& cpy) {
    *this = cpy;
}

Upload& Upload::operator=(const Upload& rhs) {

    if (this != &rhs) {
        _request = rhs._request;
        _server = rhs._server;
    }
    return *this;
}

void Upload::parseUpload() {

    std::string         body = _request.getBody();
    std::stringstream   ss(body);
    std::string         line;
    std::string         next;
    std::stringstream   res;

    bool bodyStart = false;
    std::getline(ss, line);
    while (std::getline(ss, next)) {
        if (line.find("Content-Disposition:") != std::string::npos && line.find("filename=") != std::string::npos && filename == "") {
            int pos = line.find("filename=");
            filename = line.substr(pos + 10, line.size() - pos - 12);
        } else if (line.find("Content-Type:") != std::string::npos) {
            bodyStart = true;
            std::getline(ss, next);
        } else if (bodyStart == true) {
            for (size_t i = 0; i < line.length(); ++i) {
                if (line[i] != 13 && line[i] != 10)
                    res << line[i];
            }
            if (!(next.find(_request.getContentId()) != std::string::npos)) {
                res << std::endl;
            }
        }
        line = next;
    }
    fileBody = res.str();
}

int Upload::doUpload() {

    std::string folder_name = "uploads"; // config
    if (std::system(("test -d " + std::string(folder_name)).c_str()) != 0)
        if (std::system(("mkdir " + std::string(folder_name)).c_str()) < 0)
            return 2;
    parseUpload();
    std::stringstream path;
    path << folder_name << "/" << filename;
    if (access(path.str().c_str(), F_OK) == 0) {
        int i = 0;
        while (access(path.str().c_str(), F_OK) == 0) {
            i++;
            path.str("");
            path << folder_name << "/";
            path << filename.substr(0, filename.find('.'));
            path << "(" << i << ")";
            path << filename.substr(filename.find('.'), filename.size() - filename.find('.'));
        }
    }
    std::ofstream file(path.str().c_str());
    if (file.is_open()) {
        file << fileBody;
        return 1;
    }
    return 2;
}