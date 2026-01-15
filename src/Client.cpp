#include "Client.hpp"

Client::Client(int fd, const std::string& ip, int port)
    : fd(fd), clientIP(ip), serverPort(port), bytesSent(0)
{
}

Client::~Client()
{
}

void Client::appendToBuffer(const std::string& data)
{
    requestBuffer += data;
}

std::string Client::getRequestBuffer() const
{
    return requestBuffer;
}

void Client::setResponse(const std::string& res)
{
    response = res;
    bytesSent = 0;
}

std::string Client::getResponse() const
{
    return response;
}

void Client::addBytesSent(size_t bytes)
{
    bytesSent += bytes;
}

size_t Client::getBytesSent() const
{
    return bytesSent;
}



std::string Client::getClientIP() const
{
    return clientIP;
}

int Client::getServerPort() const
{
    return serverPort;
}

int Client::getFd() const
{
    return fd;
}