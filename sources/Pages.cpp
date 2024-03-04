#include "Pages.hpp"

Pages::Pages(void)
{
    return ;
}

Pages::~Pages(void)
{
    return ;
}

bool Pages::cgiExtension(std::string file_path, std::string extension)
{
	if (file_path.length() >= extension.length()) {
		return (file_path.compare(file_path.length() - extension.length() , extension.length(), extension) == 0);
	}
	return false;
}
std::string Pages::displayPage(std::string file_path, std::string method, std::map<int, User> Users, int i)
{
	status = 200;
	message = "OK";
	std::stringstream body;
	// verifier la taille de la requete :
	if (Users[i].request.size() <= 10000) { // depend du fichier de config
		// verifier methode valide en fonction du fichier de config :
			if (method == "GET" || method == "POST") {
				if (cgiExtension(file_path, ".php") || cgiExtension(file_path, ".py")) {
					Cgi cgi;
					cgi.execCGI(file_path);
					std::ifstream file(".cgi.txt");
					body << file.rdbuf();
				} else {
					std::cout << "File or Directory" << std::endl;
					std::ifstream file(file_path.c_str());
					if (file.fail()) {
						status = 404;
						message = "Not Found";
						std::ifstream file_error("./pages/error_page_404.html");
						body << file_error.rdbuf();
					} else {
						body << file.rdbuf();
					}
				}
			} else if (method == "DELETE") {
				std::cout << "Methode DELETE" << std::endl;
			} else {
				std::cout << "Methode not implemented" << std::endl;
				status = 501;
			}
		// Si ce n'est pas une methode valide :
			// status = 405

			std::stringstream content;
			std::string data = body.str();
			clength = data.length();
			std::string file_extension = file_path.substr(file_path.find_last_of(".") + 1);

			ctype = "text/html";
			file_extension == "css" ? ctype = "text/css" : ctype = "text/html";
			content << "HTTP/1.1 " << status << " " << message << std::endl;
			content << "Content-Type: " << ctype << std::endl;
			content << "Content-Length: " << clength << std::endl << std::endl;
			content << data;

			// if (Pages::cgiExtension(file_path, ".py") || Pages::cgiExtension(file_path, ".php")) {
			// 		std::ifstream infile("outfile.txt");
			// 	if (infile) {
			// 		content << infile.rdbuf();
			// 	}
			// }
			// else {
			// 	std::ifstream file(file_path.c_str());
			// 	std::stringstream body;
			// 	std::string data;
			// 	if (file.fail())
			// 	{
			// 		std::cout << "Page not found" << std::endl;
			// 		status = 404;
			// 		message = "Not Found";
			// 		std::ifstream file_error("./pages/error_page_404.html");
			// 		body << file_error.rdbuf();
			// 		data = body.str();
			// 		clength = data.length();
			// 	}
			// 	else
			// 	{
			// 		status = 200;
			// 		message = "OK";
			// 		body << file.rdbuf();
			// 		data = body.str();
			// 		clength = data.length();
			// 	}
			// 	file.close();
			// 	std::string file_extension = file_path.substr(file_path.find_last_of(".") + 1);
			// 	file_extension == "css" ? ctype = "text/css" : ctype = "text/html";

			// 	content << "HTTP/1.1 " << status << " " << message << std::endl;
			// 	content << "Content-Type: " << ctype << std::endl;
			// 	content << "Content-Length: " << clength << std::endl << std::endl;
			// 	content << data;
			// }
			return (content.str());
	}
	std::cout << "Too large request !" << std::endl;
	return NULL;
}