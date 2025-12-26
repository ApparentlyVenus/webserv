/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:41:56 by yitani            #+#    #+#             */
/*   Updated: 2025/12/26 19:10:14 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request Request::parse(const std::string &buffer, size_t maxBodySize)
{
	Request req;

	// the end of the request is marked by "\r\n\r\n"
	if (buffer.find("\r\n\r\n") == std::string::npos)
	{
		req.state = parsingState::INCOMPLETE;
		return (req);
	}

	// extract first string
	std::string temp = buffer.substr(0, buffer.find("\r\n"));

	// first string should contain 3 parts (method, path+query, HTTP version)
	std::vector<std::string> splittedString = split(temp, ' ');
	if (splittedString.size() != 3)
	{
		req.errorCode = 400;
		req.state = parsingState::ERROR;
		return (req);
	}

	// extract method
	req.method = splittedString[0];
	if (req.method != "GET" && req.method != "POST" && req.method != "DELETE")
	{
		req.errorCode = 501;
		req.state = parsingState::ERROR;
		return (req);
	}

	// extract path and query
	std::vector<std::string> splittedPath = split(splittedString[1], '?');
	req.path = splittedPath[0];
	if (splittedPath.size() == 2)
		req.query = splittedPath[1];

	// extract HTTP version
	req.version = splittedString[2];

	// extract rest until the end of request and before Body if it exists
	std::string headerString = buffer.substr(buffer.find("\r\n") + 2, buffer.find("\r\n\r\n") - (buffer.find("\r\n") + 2));
	std::vector<std::string> splitHeaders = split(headerString, '\n');
	for (size_t i = 0; i < splitHeaders.size(); i++)
	{
		size_t colon = splitHeaders[i].find(':');
		if (colon != std::string::npos)
		{
			std::string key = splitHeaders[i].substr(0, colon);
			std::string value = splitHeaders[i].substr(colon + 1);
			req.headers[toLower(trim(key))] = trim(value);
		}
	}

	// if version is different than HTTP 1.1 -ERROR- (changeable depending on what we end up choosing)
	if (req.version != "HTTP/1.1")
	{
		req.errorCode = 505;
		req.state = parsingState::ERROR;
		return (req);
	}

	// max path lenght is 2 Kilo bytes
	if (req.path.length() > 2048)
	{
		req.errorCode = 414;
		req.state = parsingState::ERROR;
		return (req);
	}

	// if host not found then error
	if (req.headers.find("host") == req.headers.end())
	{
		req.errorCode = 400;
		req.state = parsingState::ERROR;
		return (req);
	}

	if (req.headers.find("content-length") != req.headers.end())
	{
		int contentLength = std::atoi(req.headers["content-length"].c_str());

		std::string body = buffer.substr(buffer.find("\r\n\r\n") + 4);

		// if the body exceeds max body size error
		if (body.length() > maxBodySize || (size_t)contentLength > maxBodySize)
		{
			req.errorCode = 413;
			req.state = parsingState::ERROR;
			return (req);
		}

		// if body size less then content lenght then incomplete
		if (body.length() < (size_t)contentLength)
		{
			req.state = parsingState::INCOMPLETE;
			return (req);
		}

		req.body = body.substr(0, contentLength);
	}

	req.state = parsingState::COMPLETE;
	return (req);
}
