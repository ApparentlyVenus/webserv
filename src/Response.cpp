/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by yitani            #+#    #+#             */
/*   Updated: 2026/01/16 18:02:05 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

std::string	Response::getServerName()
{
	return (this->serverName);
}
std::string	Response::getServerPort()
{
	std::stringstream	ss;
	ss << this->serverPort;
	return (ss.str());
}

std::string	Response::getClientIP()
{
	return (this->clientIP);
}

Response::Response(Request &req, const LocationConfig &conf, const ServerConfig& servConf, int port, std::string IP)
{
	std::string root = conf.getRoot();
	if (root.empty())
		root = servConf.getRoot();

	this->clientIP = IP;
	this->serverName = servConf.getServerName();
	this->serverPort = port;
	this->rootPath = root;

	std::string locationPath = conf.getPath();
	std::string requestPath = req.path;
	std::string relativePath = requestPath;

	if (requestPath.find(locationPath) == 0 && locationPath != "/")
		relativePath = requestPath.substr(locationPath.length());

	this->fullPath = root + relativePath;
	if (req.headers.find("connection") != req.headers.end())
		this->headers["Connection"] = req.headers["connection"];
	else
		this->headers["Connection"] = "close";
	if (req.state == ERROR)
	{
		this->statusCode = req.errorCode;

		if (servConf.hasErrorPage(req.errorCode))
		{
			std::string errorPagePath = servConf.getErrorPage(req.errorCode);
			
			if (fileExists(errorPagePath) && isReadable(errorPagePath))
			{
				this->body = readFile(errorPagePath);
				this->headers["Content-Type"] = getMimeType(errorPagePath);
			}
			else
			{
				std::stringstream ss;
				ss << req.errorCode;
				this->body = "<html><body><h1>" + ss.str() + " Error</h1></body></html>";
				this->headers["Content-Type"] = "text/html";
			}
		}
		else
		{
			std::stringstream ss;
			ss << req.errorCode;
			this->body = "<html><body><h1>" + ss.str() + " Error</h1></body></html>";
			this->headers["Content-Type"] = "text/html";
		}
		
		std::stringstream ss;
		ss << this->body.length();
		this->headers["Content-Length"] = ss.str();
	}
}

Response::~Response(){}

std::string Response::format(Response &res)
{
	std::stringstream stringBuilder;
	std::map<std::string, std::string>::iterator it;

	stringBuilder << "HTTP/1.1 " << res.statusCode << " "
				  << getReasonPhrase(res.statusCode) << "\r\n";

	for (it = res.headers.begin(); it != res.headers.end(); ++it)
	{
		stringBuilder << it->first << ": " << it->second << "\r\n";
	}

	for (size_t i = 0; i < res.setCookies.size(); i++)
	{
		stringBuilder << "Set-Cookie: " << res.setCookies[i] << "\r\n";
	}

	stringBuilder << "\r\n";
	stringBuilder << res.body;

	return (stringBuilder.str());
}

std::string Response::getReasonPhrase(int statusCode)
{
	switch (statusCode)
	{
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 204:
		return "No Content";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 400:
		return "Bad Request";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 413:
		return "Payload Too Large";
	case 414:
		return "URI Too Long";
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 505:
		return "HTTP Version Not Supported";
	default:
		return "Unknown";
	}
}
std::string Response::getRootPath() {
	return rootPath;
}