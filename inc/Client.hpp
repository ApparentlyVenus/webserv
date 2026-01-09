#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>


class Client
{
    private:
        int fd;
        std::string clientIP;
        int serverPort;
        std::string requestBuffer;
        std::string response;
        size_t bytesSent;

    public:
        Client(int fd, const std::string& ip, int port);
        ~Client();

        void appendToBuffer(const std::string& data);
        std::string getRequestBuffer() const;
        
        void setResponse(const std::string& res);
        std::string getResponse() const;
        
        void addBytesSent(size_t bytes);
        size_t getBytesSent() const;

        
        std::string getClientIP() const;
        int getServerPort() const;
        int getFd() const;
};

#endif