/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 09:10:18 by odana             #+#    #+#             */
/*   Updated: 2026/01/02 10:30:49 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

std::vector<int> ServerConfig::getPorts() const {
    return ports;
}

std::string ServerConfig::getIP() const {
    return ip;
}

std::string ServerConfig::getServerName() const {
    return serverName;
}

std::string ServerConfig::getRoot() const {
    return root;
}

int ServerConfig::getClientMaxBodySize() const {
    return clientMaxBodySize;
}

bool ServerConfig::hasErrorPage(int errorCode) const {
    return errorPages.find(errorCode) != errorPages.end();
}

std::string ServerConfig::getErrorPage(int errorCode) const {
    if (hasErrorPage(errorCode)) {
        return errorPages.at(errorCode);
    }
    return ""; // could throw MissingErrorPageException
}

std::vector<LocationConfig> ServerConfig::getLocations() const {
    return locations;
}

const LocationConfig* ServerConfig::matchLocation(const std::string& uri) const {
    const LocationConfig* match = NULL;
    size_t longestMatch = 0;

    for (size_t i = 0; i < locations.size(); i++) {
        std::string currentPath = locations[i].getPath();

        if (uri.find(currentPath) == 0) {
            size_t currentLength = currentPath.length();
            if (currentLength > longestMatch) {
                longestMatch = currentLength;
                match = &locations[i];
            }
        }
    }
    return match;
}

bool ServerConfig::isValid() const {
    for (size_t i = 0; i < ports.size(); i++) {
        if (ports.at(i) <= 0 || ports.at(i) > 65535)
            return false;
    }

    if (root.empty())
        return false;

    if (clientMaxBodySize <= 0)
        return false;

    for (size_t i = 0; i < locations.size(); i++) {
        if (!locations[i].isValid())
            return false;
    }

    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = i + 1; j < locations.size(); j++) {
            if (locations[i].getPath() == locations[j].getPath())
                return false;
        }
    }

    return true;
}