#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <string>
#include <map>
#include <ctime>

struct Session
{
    std::string sessionId;
    std::map<std::string, std::string> data;
    time_t createdAt;
    time_t lastAccess;
    
    // ✅ Add default constructor
    Session();
    
    // Keep your existing constructor
    Session(const std::string& id);
};

class SessionManager
{
    private:
        static std::map<std::string, Session> sessions;
        
        static std::string generateSessionId();
        static bool isSessionExpired(const Session& session, int timeoutSeconds);
        
    public:
        static std::string createSession();
        static Session* getSession(const std::string& sessionId);
        static void touchSession(const std::string& sessionId);
        static void destroySession(const std::string& sessionId);
};

#endif