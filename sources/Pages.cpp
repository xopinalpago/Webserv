#include "Pages.hpp"

Pages::Pages(void)
{
    return ;
}

Pages::~Pages(void)
{
    return ;
}

std::string Pages::displayPage(std::string file_path)
{
	// std::cout << file_path.c_str() << std::endl;
	// determiner si 
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