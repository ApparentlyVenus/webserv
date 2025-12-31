/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by yitani            #+#    #+#             */
/*   Updated: 2025/12/31 17:40:33 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response(Request &req, const LocationConfig &conf)
{
	this->fullPath = conf.getRoot() + req.path;
	if (req.headers.find("connection") != req.headers.end())
		this->headers["Connection"] = req.headers["connection"];
	else
		this->headers["Connection"] = "close";
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
