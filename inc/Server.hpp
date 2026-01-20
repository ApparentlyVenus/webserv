/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:30:24 by yitani            #+#    #+#             */
/*   Updated: 2026/01/16 17:45:52 by yitani           ###   ########.fr       */
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
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <utility>
# include <set>
# include <poll.h>
# include <iostream>
# include "../inc/Response.hpp"
# include "../inc/FileConfig.hpp"

typedef struct ListeningSocket
{
	int fd;
	std::string ip;
	int port;
} LSocket;

class Server
{
private:
	std::map<int, size_t> socketToConfig; // listening_fd to config
	std::map<int, int> clientToSocket; // lclient_fd to server_fd
	std::vector<ServerConfig> configs;
	std::vector<LSocket> LSockets;
	std::vector<struct pollfd> pollFds;
	std::map<int, Client *> clients;
	bool is_server_running;

	ServerConfig* getConfigSocket(int serverFd);
    ServerConfig* getConfigClient(int clientFd);

	int createServerSocket(int port, const std::string &ip);
	void setNonBlocking(int fd);
	void setupServerSockets();

	void acceptNewConnection(int serverFd);
	void handleClientRead(int clientFd);
	void handleClientWrite(int clientFd);
	void closeClient(int clientFd);

public:
	Server();
	Server(const FileConfig &config);
	~Server();

	void stop();
	void run();
};

#endif