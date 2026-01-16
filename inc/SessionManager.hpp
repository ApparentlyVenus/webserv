/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 11:20:15 by odana             #+#    #+#             */
/*   Updated: 2026/01/16 17:50:08 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_MANAGER_HPP
# define SESSION_MANAGER_HPP


# include <string>
# include <map>
# include <ctime>

struct Session
{
    std::string sessionId;
    std::map<std::string, std::string> data;
    time_t createdAt;
    time_t lastAccess;
    
    Session();
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