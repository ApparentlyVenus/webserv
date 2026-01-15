/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by wasamr            #+#    #+#             */
/*   Updated: 2026/01/15 19:08:12 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <set>
#include <cerrno>
#include <../inc/Handlers.hpp>
#include <../inc/ResponseStatus.hpp>

Server::Server()
{
}
Server::Server(const ServerConfig& config)
    : config(config), is_server_running(false)
{
}
ServerConfig* Server::getServerConfig()
{
    return &config;
}

Server::~Server()
{
    size_t i = 0;
    std::map<int, Client*>::iterator it = clients.begin();

    is_server_running = false;
    
    while(i < LSockets.size())
    {
        close(LSockets[i].fd);
        i++;
    }

    while (it != clients.end())
    {
        close(it->first);
        delete it->second;
        ++it;
    }
}

int Server::createServerSocket(int port, const std::string &ip)
{
    int opt = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    if (sockfd < 0)
    {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        close(sockfd);
        return -1;
    }
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (ip.empty() || ip == "0.0.0.0")
        address.sin_addr.s_addr = INADDR_ANY;
    else if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0)
    {
        std::cerr << "Error: Invalid IP address: " << ip << std::endl;
        close(sockfd);
        return -1;
    }
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Error: Bind failed on " << ip << ":" << port << std::endl;
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << "Error: Listen failed" << std::endl;
        close(sockfd);
        return -1;
    }
    return sockfd;
}

void Server::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        std::cerr << "Error: fcntl F_GETFL failed" << std::endl;
        return;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        std::cerr << "Error: fcntl F_SETFL failed" << std::endl;
        return;
    }
}


void Server::setupServerSockets()
{
    std::vector<int> ports = config.getPorts();
    std::string ip = config.getIP();
    LSocket lsock;
    struct pollfd pfd;
    size_t i = 0;


    while (i < ports.size())
    {
        int port = ports[i];
        int sockfd = createServerSocket(port, ip);
        if (sockfd < 0)
        {
            std::cerr << "Error: Failed to create socket for " << ip << ":" << port << std::endl;
            i++;
            continue;
        }

        setNonBlocking(sockfd);

        lsock.fd = sockfd;
        lsock.ip = ip;
        lsock.port = port;
        LSockets.push_back(lsock);

        pfd.fd = sockfd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollFds.push_back(pfd);
        i++;
    }

    std::cout << "Setup complete: " << LSockets.size() << " listening socket(s) created" << std::endl;
}


void Server::acceptNewConnection(int serverFd)
{
    struct sockaddr_in clientAddr;
    char clientIP[20];
    int serverPort = -1;
    struct pollfd pfd;
    socklen_t clientLen = sizeof(clientAddr);
    size_t i = 0;

    int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0)
    {
        std::cerr << "Error: accept failed" << std::endl;
        return;
    }
    setNonBlocking(clientFd);

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP,17);

    
    while (i < LSockets.size())
    {
        if (LSockets[i].fd == serverFd)
        {
            serverPort = LSockets[i].port;
            break;
        }
        i++;
    }
    Client *client = new Client(clientFd, std::string(clientIP), serverPort);
    clients[clientFd] = client;

    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    pollFds.push_back(pfd);

}

void Server::handleClientRead(int clientFd)
{
    char buffer[8192];
    ssize_t bytesRead;
    ServerConfig *servConf;
    int maxBodySize;
    Request req;
    LocationConfig loc;
    std::string responseStr; 
    size_t i =0;
    Client *client = clients[clientFd];
    if (!client)
        return;

     bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead < 0)
    {
        std::cerr << "Error: recv failed for fd=" << clientFd << std::endl;
        closeClient(clientFd);
        return;
    }

    if (bytesRead == 0)
    {
        std::cout << "Client fd=" << clientFd << " closed connection" << std::endl;
        closeClient(clientFd);
        return;
    }

    buffer[bytesRead] = '\0';
    client->appendToBuffer(std::string(buffer, bytesRead));

    servConf = getServerConfig();
    maxBodySize = servConf->getClientMaxBodySize();
    req = Request::parse(client->getRequestBuffer(), maxBodySize);

    if (req.state == INCOMPLETE)
        return;

    loc = *(servConf->matchLocation(req.path));


    Response res(req, loc, *servConf,client->getServerPort(), client->getClientIP());

    if (req.state != ERROR && req.errorCode == 0)
    {
        std::cout << "Request parsed: " << req.method << " " << req.path << std::endl;
        res = Handlers::router(res, req, loc, *servConf);
    }

    responseStr = Response::format(res);
    client->setResponse(responseStr);

    while(i < pollFds.size())
    {
        if (pollFds[i].fd == clientFd)
        {
            pollFds[i].events = POLLOUT;
            break;
        }
        i++;
    }
}


void Server::handleClientWrite(int clientFd)
{
    ssize_t sent;
    size_t remaining;
    size_t bytesSent;

    Client *client = clients[clientFd];
    if (!client)
        return;

     const std::string &response = client->getResponse();
     bytesSent = client->getBytesSent();
     remaining = response.size() - bytesSent;

    if (remaining == 0)
    {
        closeClient(clientFd);
        return;
    }

    sent = send(clientFd, response.c_str() + bytesSent, remaining, 0);

    if (sent < 0)
    {
        std::cerr << "Error: send failed for fd=" << clientFd << std::endl;
        closeClient(clientFd);
        return;
    }

    client->addBytesSent(sent);
    
    if (client->getBytesSent() >= response.size())
    {
        std::cout << "Response fully sent to fd=" << clientFd << std::endl;
        closeClient(clientFd);
    }
}

void Server::closeClient(int clientFd)
{
    std::cout << "Closing client fd=" << clientFd << std::endl;

    std::map<int, Client *>::iterator it = clients.find(clientFd);
    if (it != clients.end())
    {
        delete it->second;
        clients.erase(it);
    }

    std::vector<struct pollfd>::iterator pfd = pollFds.begin();

    while (pfd != pollFds.end())
    {
        if (pfd->fd == clientFd)
        {
            pfd = pollFds.erase(pfd);
            break;                    
        }
            ++pfd;
    }
    close(clientFd);
}

void Server::run()
{
    std::cout << "Server starting..." << std::endl;

    setupServerSockets();

    if (LSockets.empty())
    {
        std::cerr << "Error: No listening sockets created" << std::endl;
        return;
    }

    is_server_running = true;
    std::cout << "Server running. Waiting for connections..." << std::endl;

    while (is_server_running)
    {
        int pollCount = poll(&pollFds[0], pollFds.size(), 5000);

        if (pollCount < 0)
        {
            std::cerr << "Error: poll failed" << std::endl;
            break;
        }

        if (pollCount == 0)
            continue;

        size_t i = 0;
        while (i < pollFds.size())
        {
            if (pollFds[i].revents != 0)
            {
                int fd = pollFds[i].fd;
                bool isListeningSocket = false;
                size_t j = 0;
                while (j < LSockets.size())
                {
                    if (LSockets[j].fd == fd)
                    {
                        isListeningSocket = true;
                        break;
                    }
                    j++;
                }

                if (isListeningSocket)
                {
                    if (pollFds[i].revents == POLLIN)
                        acceptNewConnection(fd);
                }
                else
                {
                    if (pollFds[i].revents == POLLERR || pollFds[i].revents == POLLHUP || pollFds[i].revents == POLLNVAL)
                    {
                        std::cout << "Poll error on fd=" << fd << std::endl;
                        closeClient(fd);
                        break;
                    }
                    else if (pollFds[i].revents == POLLIN)
                    {
                        handleClientRead(fd);
                        break;
                    }
                    else if (pollFds[i].revents == POLLOUT)
                    {
                        handleClientWrite(fd);
                        break;
                    }
                }
            }
            i++;
        }
    }

    std::cout << "Server shutting down..." << std::endl;
}

void Server::stop()
{
    is_server_running = false;
}
