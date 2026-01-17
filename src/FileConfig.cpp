/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:41 by odana             #+#    #+#             */
/*   Updated: 2026/01/18 00:14:58 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/FileConfig.hpp"

FileConfig::FileConfig() {}

FileConfig::FileConfig(const FileConfig& other)
    : servers(other.servers) {}

FileConfig& FileConfig::operator=(const FileConfig& other)
{
    if (this != &other)
    {
        servers = other.servers;
    }
    return *this;
}

FileConfig::~FileConfig() {}

void FileConfig::addServer(const ServerConfig& server)
{
    servers.push_back(server);
}

std::vector<ServerConfig> FileConfig::getServers() const
{
    return servers;
}

size_t FileConfig::getServerCount() const
{
    return servers.size();
}

bool FileConfig::isValid() const
{
    // Must have at least one server
    if (servers.empty())
        return false;

    // Each server must be individually valid
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (!servers[i].isValid())
            return false;
    }

    // No conflicting bindings across servers
    if (hasConflictingBindings())
        return false;

    return true;
}

std::set<std::string> FileConfig::getAllBindings() const
{
    std::set<std::string> bindings;

    for (size_t i = 0; i < servers.size(); i++)
    {
        std::string ip = servers[i].getIP();
        std::vector<int> ports = servers[i].getPorts();

        for (size_t j = 0; j < ports.size(); j++)
        {
            std::stringstream ss;
            ss << ip << ":" << ports[j];
            bindings.insert(ss.str());
        }
    }

    return bindings;
}

bool FileConfig::hasConflictingBindings() const
{
    std::set<std::string> used;
    std::set<int> wildcardPorts; // Ports bound to 0.0.0.0
    
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::string ip = servers[i].getIP();
        std::vector<int> ports = servers[i].getPorts();
        bool isWildcard = (ip == "0.0.0.0" || ip.empty());
        
        for (size_t j = 0; j < ports.size(); j++)
        {
            int port = ports[j];
            std::stringstream ss;
            ss << ip << ":" << port;
            std::string binding = ss.str();
            
            // Check for exact duplicate binding
            if (used.find(binding) != used.end())
                return true;
            
            // Check for wildcard conflicts
            if (isWildcard)
            {
                for (std::set<std::string>::iterator it = used.begin(); 
                     it != used.end(); ++it)
                {
                    std::stringstream portCheck;
                    portCheck << ":" << port;
                    if (it->find(portCheck.str()) != std::string::npos)
                        return true;
                }
                wildcardPorts.insert(port);
            }
            else
            {
                // Specific IP conflicts with 0.0.0.0 on same port
                if (wildcardPorts.find(port) != wildcardPorts.end())
                    return true;
            }
            
            used.insert(binding);
        }
    }
    
    return false;
}