/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 09:10:18 by odana             #+#    #+#             */
/*   Updated: 2026/01/16 17:44:35 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

std::vector<int> ServerConfig::getPorts() const
{
	return ports;
}

std::string ServerConfig::getIP() const
{
	return ip;
}

std::string ServerConfig::getServerName() const
{
	return serverName;
}

std::string ServerConfig::getRoot() const
{
	return root;
}

int ServerConfig::getClientMaxBodySize() const
{
	return clientMaxBodySize;
}

bool ServerConfig::hasErrorPage(int errorCode) const
{
	return errorPages.find(errorCode) != errorPages.end();
}

std::string ServerConfig::getErrorPage(int errorCode) const
{
	if (hasErrorPage(errorCode))
	{
		return errorPages.at(errorCode);
	}
	return "";
}

std::vector<LocationConfig> ServerConfig::getLocations() const
{
	return locations;
}

const LocationConfig *ServerConfig::matchLocation(const std::string &uri) const
{
	const LocationConfig *match = NULL;
	size_t longestMatch = 0;

	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string currentPath = locations[i].getPath();

		if (uri.find(currentPath) == 0)
		{
			size_t currentLength = currentPath.length();
			if (currentLength > longestMatch)
			{
				longestMatch = currentLength;
				match = &locations[i];
			}
		}
	}
	return match;
}

bool ServerConfig::isValid() const
{
	for (size_t i = 0; i < ports.size(); i++)
	{
		if (ports.at(i) <= 0 || ports.at(i) > 65535)
			return false;
	}

	if (root.empty())
		return false;

	if (clientMaxBodySize <= 0)
		return false;

	for (size_t i = 0; i < locations.size(); i++)
	{
		if (!locations[i].isValid())
			return false;
	}

	for (size_t i = 0; i < locations.size(); i++)
	{
		for (size_t j = i + 1; j < locations.size(); j++)
		{
			if (locations[i].getPath() == locations[j].getPath())
				return false;
		}
	}

	return true;
}

void ServerConfig::addPort(int port)
{
	for (size_t i = 0; i < ports.size(); i++)
	{
		if (ports[i] == port)
			return;
	}
	ports.push_back(port);
}

void ServerConfig::setIP(const std::string &newIp)
{
	ip = newIp;
}

void ServerConfig::setServerName(const std::string &name)
{
	serverName = name;
}

void ServerConfig::setRoot(const std::string &newRoot)
{
	root = newRoot;
}

void ServerConfig::setClientMaxBodySize(int size)
{
	clientMaxBodySize = size;
}

void ServerConfig::addErrorPage(int code, const std::string &path)
{
	errorPages[code] = path;
}

void ServerConfig::addLocation(const LocationConfig &loc)
{
	locations.push_back(loc);
}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig()
	: ip("0.0.0.0"), serverName(""), root(""), clientMaxBodySize(1048576)
{
}

ServerConfig::ServerConfig(const ServerConfig &other)
	: ports(other.ports),
	  ip(other.ip),
	  serverName(other.serverName),
	  root(other.root),
	  clientMaxBodySize(other.clientMaxBodySize),
	  errorPages(other.errorPages),
	  locations(other.locations)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		ports = other.ports;
		ip = other.ip;
		serverName = other.serverName;
		root = other.root;
		clientMaxBodySize = other.clientMaxBodySize;
		errorPages = other.errorPages;
		locations = other.locations;
	}
	return *this;
}
