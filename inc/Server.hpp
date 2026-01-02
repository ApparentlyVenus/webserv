#include <string>
#include <cstring> 
#include <map>
#include <vector>
#include "../inc/ServerConfig.hpp"
#include "../inc/Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>  //for the sockaddr_in strcut
#include <arpa/inet.h>
#include <fcntl.h>
#include <utility>
#include <set>
#include <poll.h>
#include <iostream>

typedef struct ListeningSocket{
    int fd;
    std::string ip;
    int port;
}LSocket;

class Server{
    private:
        std::vector<ServerConfig> configs;
        std::map<int,std::vector<ServerConfig*> >serversByPort;
        std::vector<LSocket> LSockets;
        std::vector<struct pollfd> pollFds;
        std::map<int, Client*> clients;
        bool is_server_running;

    int createServerSocket(int port, const std::string& ip);
    void setNonBlocking(int fd); 
    void setupServerSockets(); 
    void indexServerConfigs();
    
};

