class Client {
public:
    int         fd;
    int         serverFd;       // Track which listening socket accepted this client
    std::string buffer;
    Request     request;
    std::string response;
    bool        responseReady;
    time_t      lastActivity;

    Client(int fd, int serverFd);
};
