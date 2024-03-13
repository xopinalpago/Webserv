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

    // std::cout << "PARSING" << std::endl;
    std::string body;
    std::string req = _request.getAllRequest();
    size_t header_end = req.find("\r\n\r\n");
    if (header_end != std::string::npos)
        body = req.substr(header_end + 4, req.size() - header_end - 4);
    else {
        body = "";
        return ;
    }
    // std::cout << "***********body***********" << std::endl;
    // std::cout << body << std::endl;
    // std::cout << "**************************" << std::endl;
    // std::stringstream ss(body);
    // std::string line;
    // bool bodyStart = false;
    // while (std::getline(ss, line)) {
    //     if (line.find("Content-Disposition:") != std::string::npos && line.find("filename=") != std::string::npos && filename == "") {
    //         int pos = line.find("filename=");
    //         filename = line.substr(pos + 10, line.size() - pos - 12);
    //     } else if (line.find("Content-Type:") != std::string::npos) {
    //         bodyStart = true;
    //         std::getline(ss, line);
    //     } else if (bodyStart == true) {
    //         if (line.find(_request.getContentId()) != std::string::npos) {
    //             break;
    //             // std::cout << 
    //         }
    //         fileBody += line + '\n';
    //     }
    // }

    // std::cout << "fileBody : \n" << fileBody << "|" << std::endl;
    // std::cout << "filename : " << filename << std::endl;

}

int Upload::doUpload() {

    std::string folder_name = "UpLoad"; // recuperer dans l'uri
    if (std::system(("test -d " + std::string(folder_name)).c_str()) != 0) {
        if (std::system(("mkdir " + std::string(folder_name)).c_str()) == 0) {
            parseUpload();
            // std::cout << "filename : " << filename << std::endl;
            // recuperer le nom du fichier
            // faire une boucle : (sortir que si le fichier est cree avec break)
                // si le fichier est deja dans le dossier cree :
                    // nouveau nom de fichier avec (i)
                // sinon creer le fichier
                    // break
            // recuperer le corps du fichier
            // ecrire le corps du fichier dans le fichier
        } else
            return 500; // error while creating 
    } else
        return 2; // directory already exists
    return 200;
}