#include "Utils.hpp"

Utils::Utils(void) {}

Utils::~Utils(void) {}

int Utils::fileExists(std::string filename)
{
    std::ifstream file(filename.c_str());
    if (file.fail())
	{
        file.close();
        return (1);
    }
    file.close();
    return (0);
}

int	Utils::stringToInt(std::string str)
{

	if (str.length() > 10)
		return (0);
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (std::isdigit(str[i]) == 0)
			return (0);
	}
	std::stringstream stream(str);
	int n;
	stream >> n;
	return (n);
}

std::string Utils::trim(std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

std::string Utils::trimTab(std::string& str)
{
    size_t first = str.find_first_not_of('\t');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of('\t');
    return (str.substr(first, (last - first + 1)));
}

int Utils::inVector(std::vector<std::string> vec, std::string elem)
{
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i] == elem) {
            return (0);
        }
    }
    return (1);
}
