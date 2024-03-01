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
std::string Pages::displayPage(std::string file_path, std::string method)
{
	// determiner si il s'agit d'un script cgi : (accepter en fonction du fichier de config)
	if (cgiExtension(file_path, ".php") || cgiExtension(file_path, ".py"))
	{
		// verifier que la methode est autorisee : (en fonction du fichier de config)
		// std::cout << "METHODE=" << method << std::endl;
		if (method == "GET" || method == "POST" || method == "DELETE") {
			// std::cout << "--- necessite un script CGI ---" << std::endl;
			Cgi cgi;
			cgi.execCGI(file_path);
		}

	}
	else { // ce n'est pas un script cgi
		// std::cout << "--- ne necessite pas un script CGI ---" << std::endl;
		// requete autorisee ?
		// requete GET ou POST ou DELETE ?
		if (method == "GET") {
			// std::cout << "Requete GET\n";
		} else if (method == "POST") {
			std::cout << "Requete POST\n";
		} else if(method == "DELETE") {
			std::cout << "Requete DELETE\n";
		} else {
			std::cout << "Requete NON AUTORISEE\n";
		}
	}

	if (Pages::cgiExtension(file_path, ".py") || Pages::cgiExtension(file_path, ".php"))
		file_path = "pages/index.html";
	std::ifstream file(file_path.c_str());
	std::stringstream htmlResponse;
	std::string data;
	if (file.fail())
	{
		std::cout << "Page not found" << std::endl;
		status = 404;
		message = "Not Found";
		std::ifstream file_error("./pages/error_page_404.html");
		htmlResponse << file_error.rdbuf();
		data = htmlResponse.str();
		clength = data.length();
	}
	else
	{
		status = 200;
		message = "OK";
		htmlResponse << file.rdbuf();
		data = htmlResponse.str();
		clength = data.length();
	}
	file.close();

	std::string file_extension = file_path.substr(file_path.find_last_of(".") + 1);
    file_extension == "css" ? ctype = "text/css" : ctype = "text/html";

	std::stringstream content;
	content << "HTTP/1.1 " << status << " " << message << std::endl;
	content << "Content-Type: " << ctype << std::endl;
	content << "Content-Length: " << clength << std::endl << std::endl;
	content << data;
	return (content.str());
}