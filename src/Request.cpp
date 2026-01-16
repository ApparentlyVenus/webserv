/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:41:56 by yitani            #+#    #+#             */
/*   Updated: 2026/01/16 17:47:17 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request::Request()
{
	errorCode = 0;
	state = INCOMPLETE;
}

Request::~Request(){}

static std::string	normalizePath(std::string &path)
{
	std::vector<std::string>	results;
	std::vector<std::string>	segments = split(path, '/');
	
	for (size_t i = 0; i < segments.size(); i++)
	{
		std::string seg = segments[i];
		
		if (seg == "." || seg == "")
			continue;
		else if (seg == "..")
		{
			if (!results.empty())
				results.pop_back();
		}
		else
			results.push_back(seg);
	}
	
	std::string	normalized = "/";
	
	for (size_t i = 0; i < results.size(); i++)
	{
		normalized += results[i];
		if (i < results.size() - 1)
			normalized += "/";
	}

	return (normalized);
}

Request Request::parse(const std::string &buffer, size_t maxBodySize)
{
	Request req;

	if (buffer.find("\r\n\r\n") == std::string::npos)
	{
		req.state = INCOMPLETE;
		return (req);
	}

	std::string temp = buffer.substr(0, buffer.find("\r\n"));
	std::vector<std::string> splittedString = split(temp, ' ');
	if (splittedString.size() != 3)
	{
		req.errorCode = 400;
		req.state = ERROR;
		return (req);
	}

	req.method = splittedString[0];
	if (req.method != "GET" && req.method != "POST" && req.method != "DELETE")
	{
		req.errorCode = 501;
		req.state = ERROR;
		return (req);
	}

	std::vector<std::string> splittedPath = split(splittedString[1], '?');
	req.path = normalizePath(splittedPath[0]);
	if (splittedPath.size() == 2)
		req.query = splittedPath[1];

	req.version = splittedString[2];

	std::string headerString = buffer.substr(buffer.find("\r\n") + 2, buffer.find("\r\n\r\n") - (buffer.find("\r\n") + 2));
	std::vector<std::string> splitHeaders = split(headerString, '\n');
	for (size_t i = 0; i < splitHeaders.size(); i++)
	{
		size_t colon = splitHeaders[i].find(':');
		if (colon != std::string::npos)
		{
			std::string key = splitHeaders[i].substr(0, colon);
			std::string value = splitHeaders[i].substr(colon + 1);

			if (toLower(trim(key)) == "cookie")
			{
				std::vector<std::string> splitCookies = split(value, ';');
				for (size_t j = 0; j < splitCookies.size(); j++)
				{
					size_t equal = splitCookies[j].find('=');
					if (equal != std::string::npos)
					{
						std::string cookieKey = splitCookies[j].substr(0, equal);
						std::string cookieValue = splitCookies[j].substr(equal + 1);
						req.cookies[trim(cookieKey)] = trim(cookieValue);
					}
				}
			}
			else
				req.headers[toLower(trim(key))] = trim(value);
		}
	}

	if (req.version != "HTTP/1.1")
	{
		req.errorCode = 505;
		req.state = ERROR;
		return (req);
	}

	if (req.path.length() > 2048)
	{
		req.errorCode = 414;
		req.state = ERROR;
		return (req);
	}

	if (req.headers.find("host") == req.headers.end())
	{
		req.errorCode = 400;
		req.state = ERROR;
		return (req);
	}

	if (req.headers.find("content-length") != req.headers.end())
	{
		int contentLength = std::atoi(req.headers["content-length"].c_str());

		std::string body = buffer.substr(buffer.find("\r\n\r\n") + 4);

		if (body.length() > maxBodySize || (size_t)contentLength > maxBodySize)
		{
			req.errorCode = 413;
			req.state = ERROR;
			return (req);
		}

		if (body.length() < (size_t)contentLength)
		{
			req.state = INCOMPLETE;
			return (req);
		}

		req.body = body.substr(0, contentLength);
	}

	req.state = COMPLETE;
	return (req);
}
