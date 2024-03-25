#include "User.hpp"

User::User(void)
{
	lastTime = time(NULL);
    return ;
}

User::~User(void)
{
    return ;
}

User::User(const User& cpy) {
    *this = cpy;
}

User& User::operator=(const User& rhs) {

    if (this != &rhs) {
        request = rhs.request;
        response = rhs.response;
        fd = rhs.fd;
		lastTime = rhs.lastTime;
		ServerVec = rhs.ServerVec;
    }
    return *this;
}

void	User::AddServerPtr(Server newServerVec)
{
	this->ServerVec.push_back(newServerVec);
}

void User::setFd(int fd)
{
	this->fd = fd;
	return ;
}

std::vector< Server> User::getServerVec(void) const
{
	return (this->ServerVec);
}

int User::getFd(void) const
{
	return (this->fd);
}

Request 	User::getRequest(void) const
{
	return (this->request);
}

void User::setRequest(Request request)
{
	this->request = request;
	return ;
}

Response	User::getResponse(void) const
{
	return this->response;
}

time_t     User::getLastTime() const
{
    return (lastTime);
}

void	User::updateTime()
{
    lastTime = time(NULL);
}
