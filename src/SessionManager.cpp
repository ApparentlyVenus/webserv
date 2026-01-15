#include "../inc/SessionManager.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>

Session::Session() : sessionId(""), createdAt(time(NULL)), lastAccess(time(NULL)) {}

Session::Session(const std::string& id) : sessionId(id), createdAt(time(NULL)), lastAccess(time(NULL)) {}

std::map<std::string, Session> SessionManager::sessions;

std::string SessionManager::generateSessionId()
{
	std::stringstream ss;
	ss << "sess_" << time(NULL) << "_" << rand();
	return ss.str();
}

std::string SessionManager::createSession()
{
	std::string id = generateSessionId();
	Session newSession(id);
	sessions[id] = newSession;
	return id;
}

Session *SessionManager::getSession(const std::string &sessionId)
{
	std::map<std::string, Session>::iterator it = sessions.find(sessionId);
	if (it == sessions.end())
		return NULL;

	if (isSessionExpired(it->second, 3600))
		return NULL;

	return &(it->second);
}

void SessionManager::touchSession(const std::string &sessionId)
{
	std::map<std::string, Session>::iterator it = sessions.find(sessionId);
	if (it != sessions.end())
		it->second.lastAccess = time(NULL);
}

void SessionManager::destroySession(const std::string &sessionId)
{
	sessions.erase(sessionId);
}

bool SessionManager::isSessionExpired(const Session &session, int timeoutSeconds)
{
	time_t now = time(NULL);
	return (now - session.lastAccess) > timeoutSeconds;
}
