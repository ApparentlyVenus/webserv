/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:30:24 by yitani            #+#    #+#             */
/*   Updated: 2026/01/02 21:52:06 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <cstring> 
# include <map>
# include <vector>
# include "../inc/ServerConfig.hpp"
# include "../inc/Client.hpp"
# include <sys/socket.h>
# include <netinet/in.h>  //for the sockaddr_in strcut
# include <arpa/inet.h>
# include <fcntl.h>
# include <utility>
# include <set>
# include <poll.h>
# include <iostream>
#include "../inc/Response.hpp"

typedef struct ListeningSocket
{
	int fd;
	std::string ip;
	int port;
} LSocket;

// class Server
// {
// 	private:
// 		std::vector<ServerConfig> configs;
// 		std::map<int, std::vector<ServerConfig *>> serversByPort;
// 		std::vector<LSocket> LSockets;
// 		std::vector<struct pollfd> pollFds;
// 		std::map<int, Client *> clients;
// 		bool is_server_running;

// 		int createServerSocket(int port, const std::string &ip);
// 		void setNonBlocking(int fd);
// 		void setupServerSockets();
// 		void indexServerConfigs();
//     void acceptNewConnection(int serverFd);
//     void handleClientRead(int clientFd);
//     void handleClientWrite(int clientFd);
//     void closeClient(int clientFd);
//     void cleanupTimeouts();



// 	void run();
// };



class Server
{
    private:
        std::vector<ServerConfig> configs;
        std::map<int, std::vector<ServerConfig *> > serversByPort;
        std::vector<LSocket> LSockets;
        std::vector<struct pollfd> pollFds;
        std::map<int, Client *> clients;
        bool is_server_running;

        /* ---------- Socket setup ---------- */
        int createServerSocket(int port, const std::string &ip);
        void setNonBlocking(int fd);
        void setupServerSockets();
        void indexServerConfigs();

        /* ---------- Poll loop helpers ---------- */
        void acceptNewConnection(int serverFd);
        void handleClientRead(int clientFd);
        void handleClientWrite(int clientFd);
        void closeClient(int clientFd);
        void cleanupTimeouts();

        /* ---------- Request processing ---------- */
        ServerConfig* findServerConfig(int port, const std::string& host);
        std::string processRequest(const Request& request,
                                   ServerConfig* serverConfig,
                                   const LocationConfig* locationConfig,
                                   const std::string& clientIP,
                                   int serverPort);
        std::string buildResponse(int statusCode,
                                 const std::string& body,
                                 const std::string& contentType);

    public:
        Server();
        Server(const std::vector<ServerConfig>& configs);
        ~Server();

        /* ---------- Main methods ---------- */
        void run();
        void stop();
        
        /* ---------- Configuration ---------- */
        void addConfig(const ServerConfig& config);
        void setConfigs(const std::vector<ServerConfig>& configs);
        std::vector<ServerConfig> getConfigs() const;
        
        /* ---------- Status ---------- */
        bool isRunning() const;
        size_t getClientCount() const;
};

#endif