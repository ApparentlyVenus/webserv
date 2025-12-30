/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by yitani            #+#    #+#             */
/*   Updated: 2025/12/30 17:54:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response Response::createResponse(Request &req, const LocationConfig &conf)
{
	Response res;

	res.fullPath = conf.getRoot() + req.path;
	if (req.headers.find("connection") != req.headers.end())
		res.headers["Connection"] = req.headers["connection"];
	else
		res.headers["Connection"] = "close";

	if (req.method == "POST")
		return (handlePost(req, res, conf));

	else if (req.method == "GET")
		return (handleGet(req, res, conf));

	return (handleDelete(req, res, conf));
}

static Response handleGet(Request &req, Response &res, const LocationConfig &conf)
{
	std::stringstream ss;

	if (!conf.isMethodAllowed("GET"))
	{
		res.statusCode = 405;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return res;
	}

	if (!fileExists(res.fullPath))
	{
		res.statusCode = 404;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>404 Not Found</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return res;
	}

	if (!isReadable(res.fullPath))
	{
		res.statusCode = 403;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return res;
	}

	if (isDirectory(res.fullPath))
	{
		std::string indexPath = res.fullPath;
		if (indexPath[indexPath.length() - 1] != '/')
			indexPath += "/";
		indexPath += conf.getIndex();

		if (fileExists(indexPath) && isReadable(indexPath))
		{
			res.statusCode = 200;
			res.body = readFile(indexPath);
			res.headers["Content-Type"] = getMimeType(indexPath);
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return res;
		}

		// @youssef : i will implement this later when i finish the whole structure 
		if (conf.isAutoIndex())
		{
			res.statusCode = 200;
			res.body = "<html><body><h1>Directory listing not yet implemented</h1></body></html>";
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return res;
		}

		res.statusCode = 403;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return res;
	}

	res.statusCode = 200;
	res.body = readFile(res.fullPath);
	res.headers["Content-Type"] = getMimeType(res.fullPath);
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

static Response handlePost(Request &req, Response &res, const LocationConfig &conf)
{
	std::stringstream	ss;
	std::string			uploadPath;
	std::string			fileName;

	uploadPath = conf.getUploadStore();
	size_t	pos = res.fullPath.find_last_of("/");
	fileName = res.fullPath.substr(pos + 1);

	if (!conf.isMethodAllowed("POST"))
	{
		res.statusCode = 405;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	if (!conf.isUploadEnable())
	{
		res.statusCode = 403;
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		res.headers["Content-Type"] = "text/html";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	if (!fileExists(uploadPath) || !isDirectory(uploadPath))
	{
		res.statusCode = 500;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	if (!isWritable(uploadPath))
	{
		res.statusCode = 403;
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		res.headers["Content-Type"] = "text/html";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}
	
	if (req.body.empty())
	{
		res.statusCode = 400;
		res.body = "<html><body><h1>400 Bad Request - No data</h1></body></html>";
		res.headers["Content-Type"] = "text/html";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}
	
	if (fileName.empty()
	|| fileName.find("..") != std::string::npos
	|| fileName.length() > 255)
	{
		res.statusCode = 400;
		res.body = "<html><body><h1>400 Bad Request - Invalid filename</h1></body></html>";
		res.headers["Content-Type"] = "text/html";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return res;
	}
	
	std::string		finalPath = uploadPath + "/" + fileName;
	std::ofstream	file(finalPath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		res.statusCode = 500;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	file.write(req.body.c_str(), req.body.length());
	file.close();
	res.statusCode = 201;
	res.body = "File uploaded successfully";
	res.headers["Content-Type"] = "text/plain";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);	
}

static Response handleDelete(Request &req, Response &res, const LocationConfig &conf)
{
	std::stringstream ss;

	if (!conf.isMethodAllowed("DELETE"))
	{
		res.statusCode = 405;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	if (!fileExists(res.fullPath))
	{
		res.statusCode = 404;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>404 Not Found</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	std::string dirPath = res.fullPath;

	if (isDirectory(dirPath))
	{
		res.statusCode = 409;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>409 Conflict - Cannot delete directory</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	size_t	pos = dirPath.find_last_of("/");
	dirPath = res.fullPath.substr(0, pos);
	if (!isWritable(dirPath))
	{
		res.statusCode = 403;
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		res.headers["Content-Type"] = "text/html";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}

	if (std::remove(res.fullPath.c_str()) != 0)
	{
		res.statusCode = 500;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		ss << res.body.length();
		res.headers["Content-Length"] = ss.str();
		return (res);
	}
	
	res.statusCode = 204;
	res.body = "";
	res.headers["Content-Length"] = "0";
	return (res);
}

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
