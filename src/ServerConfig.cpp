/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 10:30:58 by yitani            #+#    #+#             */
/*   Updated: 2026/01/02 00:45:59 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig() : ip("0.0.0.0"), serverName(""),
							   root("./www"), clientMaxBodySize(1048576) {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: ports(other.ports), ip(other.ip), serverName(other.serverName),
	  root(other.root), clientMaxBodySize(other.clientMaxBodySize),
	  errorPages(other.errorPages), locations(other.locations) {}

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

ServerConfig::~ServerConfig() {}

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
	std::map<int, std::string>::const_iterator it = errorPages.find(errorCode);
	if (it != errorPages.end())
		return it->second;
	return "";
}

const LocationConfig *ServerConfig::matchLocation(const std::string &uri) const
{
	const LocationConfig *bestMatch = 0;
	size_t longestMatch = 0;

	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string locPath = locations[i].getPath();

		if (uri.find(locPath) == 0)
		{
			if (locPath.length() > longestMatch)
			{
				longestMatch = locPath.length();
				bestMatch = &locations[i];
			}
		}
	}
	return bestMatch;
}

std::vector<LocationConfig> ServerConfig::getLocations() const
{
	return locations;
}

bool ServerConfig::isValid() const
{
	if (ports.empty())
		return false;

	for (size_t i = 0; i < ports.size(); i++)
	{
		if (ports.at(i) <= 0 || ports.at(i) > 65535)
			return false;
	}

	if (root.empty())
		return false;

	if (clientMaxBodySize < 0)
		return false;

	for (size_t i = 0; i < locations.size(); i++)
	{
		if (!locations[i].isValid())
			return false;
	}

	return true;
}

void ServerConfig::addPort(int p)
{
	ports.push_back(p);
}

void ServerConfig::setIP(const std::string &i)
{
	ip = i;
}

void ServerConfig::setServerName(const std::string &name)
{
	serverName = name;
}

void ServerConfig::setRoot(const std::string &r)
{
	root = r;
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