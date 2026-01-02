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
#include <ctime>
#include <../inc/Handlers.hpp>
/* ========================================================================== */
/*                            CONSTRUCTORS/DESTRUCTOR                         */
/* ========================================================================== */

Server::Server() : is_server_running(false)
{
}

Server::Server(const std::vector<ServerConfig>& configs) 
    : configs(configs), is_server_running(false)
{
}

Server::~Server()
{
    stop();
    
    // Close all listening sockets
    for (size_t i = 0; i < LSockets.size(); i++)
    {
        close(LSockets[i].fd);
    }
    
    // Delete all clients
    for (std::map<int, Client *>::iterator it = clients.begin(); 
         it != clients.end(); ++it)
    {
        close(it->first);
        delete it->second;
    }
}

/* ========================================================================== */
/*                              SOCKET SETUP                                  */
/* ========================================================================== */

int Server::createServerSocket(int port, const std::string &ip)
{
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return -1;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        close(sockfd);
        return -1;
    }

    // Setup address structure
    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    // Convert IP address from string to binary form
    if (ip.empty() || ip == "0.0.0.0")
        address.sin_addr.s_addr = INADDR_ANY;
    else if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0)
    {
        std::cerr << "Error: Invalid IP address: " << ip << std::endl;
        close(sockfd);
        return -1;
    }

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Error: Bind failed on " << ip << ":" << port << std::endl;
        close(sockfd);
        return -1;
    }

    // Start listening
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << "Error: Listen failed" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Server socket created and listening on " << ip << ":" << port << std::endl;
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
    std::set<std::pair<std::string, int> > uniqueSockets;

    // Collect all unique port/IP combinations
    for (size_t i = 0; i < configs.size(); i++)
    {
        std::vector<int> ports = configs[i].getPorts();
        std::string ip = configs[i].getIP();
        
        // If no IP specified, use default
        if (ip.empty())
            ip = "0.0.0.0";

        for (size_t j = 0; j < ports.size(); j++)
        {
            uniqueSockets.insert(std::make_pair(ip, ports[j]));
        }
    }

    // Create a socket for each unique port/IP combination
    for (std::set<std::pair<std::string, int> >::iterator it = uniqueSockets.begin();
         it != uniqueSockets.end(); ++it)
    {
        std::string ip = it->first;
        int port = it->second;

        int sockfd = createServerSocket(port, ip);
        if (sockfd < 0)
        {
            std::cerr << "Error: Failed to create socket for " << ip << ":" << port << std::endl;
            continue;
        }

        // Set socket to non-blocking
        setNonBlocking(sockfd);

        // Store socket information
        LSocket lsock;
        lsock.fd = sockfd;
        lsock.ip = ip;
        lsock.port = port;
        LSockets.push_back(lsock);

        // Add to poll file descriptors
        struct pollfd pfd;
        pfd.fd = sockfd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pollFds.push_back(pfd);
    }

    std::cout << "Setup complete: " << LSockets.size() << " listening socket(s) created" << std::endl;
}

void Server::indexServerConfigs()
{
    serversByPort.clear();

    for (size_t i = 0; i < configs.size(); i++)
    {
        std::vector<int> ports = configs[i].getPorts();
        
        for (size_t j = 0; j < ports.size(); j++)
        {
            int port = ports[j];
            serversByPort[port].push_back(&configs[i]);
        }
    }

    std::cout << "Indexed " << configs.size() << " server config(s) across " 
              << serversByPort.size() << " port(s)" << std::endl;
}

/* ========================================================================== */
/*                            POLL LOOP HELPERS                               */
/* ========================================================================== */

void Server::acceptNewConnection(int serverFd)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    
    int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "Error: accept failed" << std::endl;
        return;
    }

    // Set client socket to non-blocking
    setNonBlocking(clientFd);

    // Get client IP address
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

    // Find which port this connection came from
    int serverPort = -1;
    for (size_t i = 0; i < LSockets.size(); i++)
    {
        if (LSockets[i].fd == serverFd)
        {
            serverPort = LSockets[i].port;
            break;
        }
    }

    // Create new client object
    Client *client = new Client(clientFd, std::string(clientIP), serverPort);
    clients[clientFd] = client;

    // Add to poll file descriptors
    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    pollFds.push_back(pfd);

    std::cout << "New connection: fd=" << clientFd 
              << " from " << clientIP << " on port " << serverPort << std::endl;
}

void Server::handleClientRead(int clientFd)
{
    Client *client = clients[clientFd];
    if (!client)
        return;

    char buffer[8192];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            std::cerr << "Error: recv failed for fd=" << clientFd << std::endl;
            closeClient(clientFd);
        }
        return;
    }

    if (bytesRead == 0)
    {
        std::cout << "Client fd=" << clientFd << " closed connection" << std::endl;
        closeClient(clientFd);
        return;
    }

    // Append data and update activity
    buffer[bytesRead] = '\0';
    client->appendToBuffer(std::string(buffer, bytesRead));
    client->updateLastActivity();

    // Find server config
    ServerConfig *servConf = findServerConfig(client->getServerPort(), "");
    if (!servConf)
    {
        std::string errorResponse = buildResponse(500, 
            "<html><body><h1>500 Internal Server Error</h1></body></html>",
            "text/html");
        client->setResponse(errorResponse);
        client->setState(CLIENT_WRITING);
        
        // Switch to POLLOUT
        for (size_t i = 0; i < pollFds.size(); i++)
        {
            if (pollFds[i].fd == clientFd)
            {
                pollFds[i].events = POLLOUT;
                break;
            }
        }
        return;
    }

    // Parse request
    size_t maxBodySize = servConf->getClientMaxBodySize();
    Request req = Request::parse(client->getRequestBuffer(), maxBodySize);
    
    // If parsing incomplete, continue waiting for more data
    if (req.state == INCOMPLETE)
        return; // Need more data

    // Match server config based on Host header
    std::string hostHeader = req.headers.count("host") ? req.headers["host"] : "";
    servConf = findServerConfig(client->getServerPort(), hostHeader);
    
    // Match location
    const LocationConfig *loc = servConf->matchLocation(req.path);
    if (!loc)
    {
        // No location matched - create minimal location config
        LocationConfig defaultLoc;
        defaultLoc.setRoot(servConf->getRoot());
        
        // Create response with 404
        Response res(req, defaultLoc, *servConf, 
                    client->getServerPort(), client->getClientIP());
        res.statusCode = 404;
        res.body = "<html><body><h1>404 Not Found</h1></body></html>";
        res.headers["Content-Type"] = "text/html";
        std::stringstream ss;
        ss << res.body.length();
        res.headers["Content-Length"] = ss.str();
        
        std::string responseStr = Response::format(res);
        client->setResponse(responseStr);
        client->setState(CLIENT_WRITING);
        
        // Switch to POLLOUT
        for (size_t i = 0; i < pollFds.size(); i++)
        {
            if (pollFds[i].fd == clientFd)
            {
                pollFds[i].events = POLLOUT;
                break;
            }
        }
        return;
    }
    
    // Create response object
    Response res(req, *loc, *servConf, 
                client->getServerPort(), client->getClientIP());
    
    // If request has no parsing error, route through handlers
    if (req.state != ERROR && req.errorCode == 0)
    {
        std::cout << "Request parsed: " << req.method << " " << req.path << std::endl;
        
        // Call router to handle the request
        res = Handlers::router(res, req, *loc);
    }
    // else: Response constructor already handled the error case
    
    // Format response to string
    std::string responseStr = Response::format(res);
    client->setResponse(responseStr);
    client->setState(CLIENT_WRITING);
    
    // Switch to POLLOUT
    for (size_t i = 0; i < pollFds.size(); i++)
    {
        if (pollFds[i].fd == clientFd)
        {
            pollFds[i].events = POLLOUT;
            break;
        }
    }
}
void Server::handleClientWrite(int clientFd)
{
    Client *client = clients[clientFd];
    if (!client)
        return;

    const std::string &response = client->getResponse();
    size_t bytesSent = client->getBytesSent();
    size_t remaining = response.size() - bytesSent;

    if (remaining == 0)
    {
        closeClient(clientFd);
        return;
    }

    ssize_t sent = send(clientFd, response.c_str() + bytesSent, remaining, 0);

    if (sent < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            std::cerr << "Error: send failed for fd=" << clientFd << std::endl;
            closeClient(clientFd);
        }
        return;
    }

    client->addBytesSent(sent);
    
    // Check if all data has been sent
    if (client->getBytesSent() >= response.size())
    {
        std::cout << "Response fully sent to fd=" << clientFd << std::endl;
        closeClient(clientFd);
    }
}

void Server::closeClient(int clientFd)
{
    std::cout << "Closing client fd=" << clientFd << std::endl;

    // Remove from clients map
    std::map<int, Client *>::iterator it = clients.find(clientFd);
    if (it != clients.end())
    {
        delete it->second;
        clients.erase(it);
    }

    // Remove from pollFds
    for (std::vector<struct pollfd>::iterator pfd = pollFds.begin(); 
         pfd != pollFds.end(); ++pfd)
    {
        if (pfd->fd == clientFd)
        {
            pollFds.erase(pfd);
            break;
        }
    }

    // Close socket
    close(clientFd);
}

void Server::cleanupTimeouts()
{
    time_t currentTime = time(NULL);
    const int TIMEOUT_SECONDS = 60;

    std::vector<int> toClose;

    // Find timed out clients
    for (std::map<int, Client *>::iterator it = clients.begin(); 
         it != clients.end(); ++it)
    {
        Client *client = it->second;
        if (difftime(currentTime, client->getLastActivity()) > TIMEOUT_SECONDS)
        {
            std::cout << "Client fd=" << it->first << " timed out" << std::endl;
            toClose.push_back(it->first);
        }
    }

    // Close timed out clients
    for (size_t i = 0; i < toClose.size(); i++)
    {
        closeClient(toClose[i]);
    }
}

/* ========================================================================== */
/*                           REQUEST PROCESSING                               */
/* ========================================================================== */

ServerConfig* Server::findServerConfig(int port, const std::string& host)
{
    std::vector<ServerConfig *> *possibleConfigs = &serversByPort[port];
    
    if (possibleConfigs->empty())
        return NULL;
    
    // Try to match based on Host header
    for (size_t i = 0; i < possibleConfigs->size(); i++)
    {
        ServerConfig *conf = (*possibleConfigs)[i];
        if (host.find(conf->getServerName()) != std::string::npos)
            return conf;
    }
    
    // Return first config as default
    return (*possibleConfigs)[0];
}

std::string Server::processRequest(const Request& request,
                                   ServerConfig* serverConfig,
                                   const LocationConfig* locationConfig,
                                   const std::string& clientIP,
                                   int serverPort)
{
    if (!serverConfig || !locationConfig)
    {
        return buildResponse(500, 
                           "<html><body><h1>500 Internal Server Error</h1></body></html>",
                           "text/html");
    }
    
    // Create response object
    Response res(const_cast<Request&>(request), *locationConfig, 
                *serverConfig, serverPort, clientIP);
    
    // If no parsing error, route through handlers
    if (request.state != ERROR && request.errorCode == 0)
    {
        res = Handlers::router(res, const_cast<Request&>(request), *locationConfig);
    }
    
    // Format and return response string
    return Response::format(res);
}

std::string Server::buildResponse(int statusCode,
                                 const std::string& body,
                                 const std::string& contentType)
{
    std::ostringstream oss;
    
    oss << "HTTP/1.1 " << statusCode << " " 
        << Response::getReasonPhrase(statusCode) << "\r\n";
    oss << "Content-Type: " << contentType << "\r\n";
    oss << "Content-Length: " << body.length() << "\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    oss << body;
    
    return oss.str();
}

/* ========================================================================== */
/*                              MAIN LOOP                                     */
/* ========================================================================== */

void Server::run()
{
    std::cout << "Server starting..." << std::endl;
    
    // Setup all server sockets
    indexServerConfigs();
    setupServerSockets();
    
    if (LSockets.empty())
    {
        std::cerr << "Error: No listening sockets created" << std::endl;
        return;
    }

    is_server_running = true;
    time_t lastCleanup = time(NULL);
    
    std::cout << "Server running. Waiting for connections..." << std::endl;

    while (is_server_running)
    {
        // Poll with 5 second timeout
        int pollCount = poll(&pollFds[0], pollFds.size(), 5000);

        if (pollCount < 0)
        {
            if (errno == EINTR)
                continue;
            std::cerr << "Error: poll failed" << std::endl;
            break;
        }

        if (pollCount == 0)
        {
            // Timeout - check for client timeouts
            time_t now = time(NULL);
            if (difftime(now, lastCleanup) > 10)
            {
                cleanupTimeouts();
                lastCleanup = now;
            }
            continue;
        }

        // Process all ready file descriptors
        for (size_t i = 0; i < pollFds.size(); i++)
        {
            if (pollFds[i].revents == 0)
                continue;

            int fd = pollFds[i].fd;

            // Check if this is a listening socket
            bool isListeningSocket = false;
            for (size_t j = 0; j < LSockets.size(); j++)
            {
                if (LSockets[j].fd == fd)
                {
                    isListeningSocket = true;
                    break;
                }
            }

            if (isListeningSocket)
            {
                if (pollFds[i].revents & POLLIN)
                {
                    acceptNewConnection(fd);
                }
            }
            else
            {
                if (pollFds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
                {
                    std::cout << "Poll error on fd=" << fd << std::endl;
                    closeClient(fd);
                    break;
                }
                else if (pollFds[i].revents & POLLIN)
                {
                    handleClientRead(fd);
                    break;
                }
                else if (pollFds[i].revents & POLLOUT)
                {
                    handleClientWrite(fd);
                    break;
                }
            }
        }
    }

    std::cout << "Server shutting down..." << std::endl;
}

void Server::stop()
{
    is_server_running = false;
}

/* ========================================================================== */
/*                            CONFIGURATION                                   */
/* ========================================================================== */

void Server::addConfig(const ServerConfig& config)
{
    configs.push_back(config);
}

void Server::setConfigs(const std::vector<ServerConfig>& newConfigs)
{
    configs = newConfigs;
}

std::vector<ServerConfig> Server::getConfigs() const
{
    return configs;
}

/* ========================================================================== */
/*                               STATUS                                       */
/* ========================================================================== */

bool Server::isRunning() const
{
    return is_server_running;
}

size_t Server::getClientCount() const
{
    return clients.size();
}



