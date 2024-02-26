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
	std::ifstream file(file_path.c_str());

	std::stringstream htmlResponse;
	htmlResponse << file.rdbuf();
	file.close();
	std::string data = htmlResponse.str();

	status = 200;
	message = "OK";
	ctype = "text/html";
	clength = data.length();
	std::stringstream content;
	content << "HTTP/1.1 " << status << " " << message << std::endl;
	content << "Content-Type: " << ctype << std::endl;
	content << "Content-Length: " << clength << std::endl << std::endl;
	content << data;
	return (content.str());
}