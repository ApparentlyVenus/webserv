/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResonseStatus.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 19:11:29 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 19:33:33 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseStatus.hpp"

Response checkMethodAllowed(Response &res, const ServerConfig &servConf)
{
	std::stringstream	ss;
	res.statusCode = 405;
	if (servConf.hasErrorPage(405))
	{
		std::string errorPagePath = servConf.getErrorPage(405);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	NotFound(Response &res, const ServerConfig &servConf)
{
	std::stringstream	ss;
	res.statusCode = 404;
	if (servConf.hasErrorPage(404))
	{
		std::string errorPagePath = servConf.getErrorPage(404);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>404 Not Found</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	Forbidden(Response &res, const ServerConfig &servConf)
{
	std::stringstream	ss;

	if (servConf.hasErrorPage(403))
	{
		std::string errorPagePath = servConf.getErrorPage(403);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}
	res.statusCode = 403;
	res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
	res.headers["Content-Type"] = "text/html";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	GetSuccess(Response &res, const std::string& filepath)
{
	std::stringstream	ss;

	res.statusCode = 200;
	res.body = readFile(filepath);
	res.headers["Content-Type"] = getMimeType(filepath);
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response DirListing(Response &res, std::string ReqPath, const ServerConfig &servConf)
{
	std::stringstream	ss;
	DIR	*dir = opendir(res.fullPath.c_str());
	if (dir == NULL)
		return InternalServerError(res, servConf);
	struct dirent *entry;

	ss << "<html><head><title>Index of " << ReqPath << "</title></head>";
	ss << "<body><h1>Index of " << ReqPath << "</h1>";
	ss << "<ul>";
	
	while (true)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break;

		std::string temp = entry->d_name;

		if (temp == "." || temp == "..")
			continue;

		std::string displayName = temp;

		if (isDirectory(res.fullPath + "/" + temp))
			displayName += "/";

		std::string href = ReqPath;
		if (href[href.length() - 1] != '/')
			href += "/";
		href += temp;
		
		ss << "<li><a href=\"" << href << "\">" << displayName << "</a></li>";
	}
	
	ss << "</ul></body></html>";

	res.body = ss.str();
	closedir(dir);
	
	ss.str("");
	ss.clear();
	res.statusCode = 200;
	res.headers["Content-Type"] = "text/html";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	
	return (res);
}

Response	InternalServerError(Response &res, const ServerConfig &servConf)
{
	std::stringstream	ss;
	res.statusCode = 500;
	if (servConf.hasErrorPage(500))
	{
		std::string errorPagePath = servConf.getErrorPage(500);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}	
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	Conflict(Response &res, const ServerConfig &servConf)
{
	std::stringstream	ss;
	res.statusCode = 409;
	if (servConf.hasErrorPage(409))
	{
		std::string errorPagePath = servConf.getErrorPage(409);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>409 Conflict - Cannot delete directory</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	BadRequest(Response &res, const std::string	&body, const ServerConfig &servConf)
{
	std::stringstream	ss;

	res.statusCode = 400;

	if (servConf.hasErrorPage(400))
	{
		std::string errorPagePath = servConf.getErrorPage(400);
		std::string errorContent = readFile(errorPagePath);

		if (!errorContent.empty())
		{
			res.body = errorContent;
			res.headers["Content-Type"] = "text/html";
			ss << res.body.length();
			res.headers["Content-Length"] = ss.str();
			return (res);
		}
	}
	res.body = body;
	res.headers["Content-Type"] = "text/html";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	PostSuccess(Response &res)
{
	std::stringstream	ss;

	res.statusCode = 201;
	res.body = "File uploaded successfully";
	res.headers["Content-Type"] = "text/plain";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	DeleteSuccess(Response &res)
{
	res.statusCode = 204;
	res.body = "";
	res.headers["Content-Length"] = "0";
	return (res);
}
