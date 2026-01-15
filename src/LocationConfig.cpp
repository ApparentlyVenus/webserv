/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:05:08 by odana             #+#    #+#             */
/*   Updated: 2026/01/15 19:21:27 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/LocationConfig.hpp"
#include "../inc/ServerConfig.hpp"

LocationConfig::LocationConfig()
	: path(""), root(""), autoIndex(false), index("index.html"),
	  redirect(""), redirectCode(0), uploadEnable(false),
	  uploadStore(""), cgiPy(""), cgiPhp(""), cgiPl("") {}

LocationConfig::LocationConfig(const LocationConfig &other)
	: path(other.path), allowedMethods(other.allowedMethods),
	  root(other.root), autoIndex(other.autoIndex), index(other.index),
	  redirect(other.redirect), redirectCode(other.redirectCode),
	  uploadEnable(other.uploadEnable), uploadStore(other.uploadStore),
	  cgiExtension(other.cgiExtension), cgiPy(other.cgiPy), cgiPhp(other.cgiPhp), cgiPl(other.cgiPl) {}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		path = other.path;
		allowedMethods = other.allowedMethods;
		root = other.root;
		autoIndex = other.autoIndex;
		index = other.index;
		redirect = other.redirect;
		redirectCode = other.redirectCode;
		uploadEnable = other.uploadEnable;
		uploadStore = other.uploadStore;
		cgiExtension = other.cgiExtension;
		cgiPy = other.cgiPy;
		cgiPl = other.cgiPl;
		cgiPhp = other.cgiPhp;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

std::string LocationConfig::getPath() const
{
	return path;
}

std::vector<std::string> LocationConfig::getAllowedMethods() const
{
	return allowedMethods;
}

std::string LocationConfig::getRoot() const
{
	return root;
}

std::string LocationConfig::getIndex() const
{
	return index;
}

std::string LocationConfig::getUploadStore() const
{
	return uploadStore;
}

std::vector<std::string> LocationConfig::getCgiExtensions() const
{
	return cgiExtension;
}

std::string LocationConfig::getCgiPy() const
{
	return cgiPy;
}

std::string LocationConfig::getCgiPhp() const
{
	return cgiPhp;
}

std::string LocationConfig::getCgiPl() const
{
	return cgiPl;
}

bool LocationConfig::hasRedirect() const
{
	return !redirect.empty();
}

std::string LocationConfig::getRedirect() const
{
	return redirect;
}

int LocationConfig::getRedirectCode() const
{
	return redirectCode;
}

bool LocationConfig::isUploadEnable() const
{
	return uploadEnable;
}

bool LocationConfig::isAutoIndex() const
{
	return autoIndex;
}

bool LocationConfig::isMethodAllowed(const std::string &method) const
{
	if (allowedMethods.empty())
		return true;

	for (size_t i = 0; i < allowedMethods.size(); i++)
	{
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

bool LocationConfig::isCGI(const std::string &path) const
{
	if (cgiExtension.empty())
		return (false);

	size_t dot = path.find_last_of('.');
	if (dot == std::string::npos)
		return (false);

	std::string ext = path.substr(dot);

	for (size_t i = 0; i < cgiExtension.size(); i++)
	{
		if (ext == cgiExtension[i])
			return (true);
	}
	return (false);
}

std::string LocationConfig::getTrueRoot(const ServerConfig &server) const
{
	if (!root.empty())
		return root;
	return server.getRoot();
}

bool LocationConfig::isValid() const
{
	if (path.empty() || path[0] != '/')
		return false;

	if (uploadEnable && uploadStore.empty())
		return false;

	if (!cgiExtension.empty())
	{
		bool hasPy = false;
		bool hasPhp = false;
		bool hasPl = false;

		for (size_t i = 0; i < cgiExtension.size(); i++)
		{
			if (cgiExtension[i] == ".py")
				hasPy = true;
			if (cgiExtension[i] == ".php")
				hasPhp = true;
			if (cgiExtension[i] == ".pl")
				hasPl = true;
		}

		if (hasPy && cgiPy.empty())
			return false;
		if (hasPhp && cgiPhp.empty())
			return false;
		if (hasPl && cgiPl.empty())
			return false;
	}

	if (!redirect.empty() && (redirectCode != 301 && redirectCode != 302))
		return false;

	return true;
}

void LocationConfig::setPath(const std::string &p)
{
	path = p;
}

void LocationConfig::addAllowedMethod(const std::string &method)
{
	allowedMethods.push_back(method);
}

void LocationConfig::setRoot(const std::string &r)
{
	root = r;
}

void LocationConfig::setAutoIndex(bool enable)
{
	autoIndex = enable;
}

void LocationConfig::setIndex(const std::string &idx)
{
	index = idx;
}

void LocationConfig::setRedirect(const std::string &url, int code)
{
	redirect = url;
	redirectCode = code;
}

void LocationConfig::setUploadEnable(bool enable)
{
	uploadEnable = enable;
}

void LocationConfig::setUploadStore(const std::string &store)
{
	uploadStore = store;
}

void LocationConfig::addCgiExtension(const std::string &ext)
{
	cgiExtension.push_back(ext);
}

void LocationConfig::setCgiPy(const std::string &p)
{
	cgiPy = p;
}

void LocationConfig::setCgiPl(const std::string &p)
{
	cgiPl = p;
}
void LocationConfig::setCgiPhp(const std::string &p)
{
	cgiPhp = p;
}

std::string LocationConfig::getCgiInterpreter(const std::string &extension) const
{
	if (extension == ".py")
		return cgiPy;
	if (extension == ".php")
		return cgiPhp;
	if (extension == ".pl")
		return cgiPl;
	return "";
}