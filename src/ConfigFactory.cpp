/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:05:00 by odana             #+#    #+#             */
/*   Updated: 2026/01/02 00:45:18 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigFactory.hpp"

ConfigFactory::ConfigFactory() {}

ConfigFactory::ConfigFactory(const ConfigFactory &other)
{
	(void)other;
}

ConfigFactory &ConfigFactory::operator=(const ConfigFactory &other)
{
	(void)other;
	return *this;
}

ConfigFactory::~ConfigFactory() {}

void ConfigFactory::processServerDirective(ServerConfig &config, const Directive &dir)
{
	std::string key = dir.key.val;

	if (key == "listen" || key == "port")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("listen directive requires exactly one value");
		config.addPort(toInt(dir.values[0].val));
	}
	else if (key == "host")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("host directive requires exactly one value");
		config.setIP(dir.values[0].val);
	}
	else if (key == "server_name")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("server_name directive requires exactly one value");
		config.setServerName(dir.values[0].val);
	}
	else if (key == "root")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("root directive requires exactly one value");
		config.setRoot(dir.values[0].val);
	}
	else if (key == "client_max_body_size")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("client_max_body_size directive requires exactly one value");
		config.setClientMaxBodySize(toInt(dir.values[0].val));
	}
	else if (key == "error_page")
	{
		if (dir.values.size() != 2)
			throw std::runtime_error("error_page directive requires exactly two values");
		int code = toInt(dir.values[0].val);
		config.addErrorPage(code, dir.values[1].val);
	}
	else
	{
		throw std::runtime_error("Unknown server directive: " + key);
	}
}

void ConfigFactory::processLocationDirective(LocationConfig &config, const Directive &dir)
{
	std::string key = dir.key.val;

	if (key == "allowed_methods")
	{
		for (size_t i = 0; i < dir.values.size(); i++)
		{
			config.addAllowedMethod(dir.values[i].val);
		}
	}
	else if (key == "root")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("root directive requires exactly one value");
		config.setRoot(dir.values[0].val);
	}
	else if (key == "autoindex")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("autoindex directive requires exactly one value");
		config.setAutoIndex(toBool(dir.values[0].val));
	}
	else if (key == "index")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("index directive requires exactly one value");
		config.setIndex(dir.values[0].val);
	}
	else if (key == "redirect")
	{
		if (dir.values.size() != 2)
			throw std::runtime_error("redirect directive requires exactly two values");
		int code = toInt(dir.values[0].val);
		config.setRedirect(dir.values[1].val, code);
	}
	else if (key == "upload_enable")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("upload_enable directive requires exactly one value");
		config.setUploadEnable(toBool(dir.values[0].val));
	}
	else if (key == "upload_store")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("upload_store directive requires exactly one value");
		config.setUploadStore(dir.values[0].val);
	}
	else if (key == "cgi_extension")
	{
		for (size_t i = 0; i < dir.values.size(); i++)
		{
			config.addCgiExtension(dir.values[i].val);
		}
	}
	else if (key == "cgi_py")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("cgi_py directive requires exactly one value");
		config.setCgiPy(dir.values[0].val);
	}
	else if (key == "cgi_php")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("cgi_php directive requires exactly one value");
		config.setCgiPhp(dir.values[0].val);
	}
	else if (key == "cgi_pl")
	{
		if (dir.values.size() != 1)
			throw std::runtime_error("cgi_pl directive requires exactly one value");
		config.setCgiPl(dir.values[0].val);
	}
	else
	{
		throw std::runtime_error("Unknown location directive: " + key);
	}
}

void ConfigFactory::validateServerConfig(const ServerConfig &config)
{
	if (!config.isValid())
		throw std::runtime_error("Invalid server configuration");
}

void ConfigFactory::validateLocationConfig(const LocationConfig &config)
{
	if (!config.isValid())
		throw std::runtime_error("Invalid location configuration");
}

ServerConfig ConfigFactory::buildServer(const ServerBlock &block)
{
	ServerConfig config;

	for (size_t i = 0; i < block.directives.size(); i++)
	{
		processServerDirective(config, block.directives[i]);
	}

	for (size_t i = 0; i < block.locations.size(); i++)
	{
		LocationConfig loc = buildLocation(block.locations[i]);
		config.addLocation(loc);
	}

	validateServerConfig(config);
	return config;
}

LocationConfig ConfigFactory::buildLocation(const LocationBlock &block)
{
	LocationConfig config;

	config.setPath(block.path.val);

	for (size_t i = 0; i < block.directives.size(); i++)
	{
		processLocationDirective(config, block.directives[i]);
	}

	validateLocationConfig(config);
	return config;
}
