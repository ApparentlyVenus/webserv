/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 11:20:15 by odana             #+#    #+#             */
/*   Updated: 2026/01/02 11:53:40 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_MANAGER_HPP
# define SESSION_MANAGER_HPP

# include <string>
# include <map>

struct Session {
    std::string sessionId;
    time_t createdAt;
    time_t lastAccess;
    std::map<std::string, std::string> data;

    Session(const std::string& id);

};

class SessionManager {
    private:
    
    static std::map<std::string, Session> sessions;    
    SessionManager();

    public:
    
    static std::string generateSessionId();
    static std::string createSession();
    static Session* getSession(const std::string& sessionId);
    
    static void touchSession(const std::string& sessionId);
    static void destroySession(const std::string& sessionId);
    static bool isSessionExpired(const Session& session, int timeout);
};

#endif