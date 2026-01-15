/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:30:24 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 17:29:36 by yitani           ###   ########.fr       */
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


class Server
{
    private:
        ServerConfig config; //single server block
        std::vector<LSocket> LSockets;//vector of all open sockets
        std::vector<struct pollfd> pollFds;
        std::map<int, Client *> clients;
        bool is_server_running;

       int createServerSocket(int port, const std::string &ip);
        void setNonBlocking(int fd);
        void setupServerSockets();

        void acceptNewConnection(int serverFd);
        void handleClientRead(int clientFd);
        void handleClientWrite(int clientFd);
        void closeClient(int clientFd);

        ServerConfig* getServerConfig();

    public:
        Server();
        Server(const ServerConfig& config);
        ~Server();

		void stop();
        void run();
};

#endif