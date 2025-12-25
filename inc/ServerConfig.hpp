/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 09:10:20 by odana             #+#    #+#             */
/*   Updated: 2025/12/22 10:22:30 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <vector>
# include <string>
# include <map>
# include "LocationConfig.hpp"

class ServerConfig {
    private:

    int port;
    std::string ip;
    std::string serverName;
    std::string root;
    int clientMaxBodySize;
    std::map<int, std::string> errorPages;
    std::vector<LocationConfig> locations;

    public:

    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

    int getPort() const;
    std::string getIP() const;
    std::string getServerName() const;
    std::string getRoot() const;
    int getClientMaxBodySize() const;

    bool hasErrorPage(int errorCode) const;
    std::string getErrorPage(int errorCode) const;

    const LocationConfig* matchLocation(const std::string& uri) const;
    std::vector<LocationConfig> getLocations() const;
    
    bool isValid() const;
};

#endif