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
