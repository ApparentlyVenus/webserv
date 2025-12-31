/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResonseStatus.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 19:11:29 by yitani            #+#    #+#             */
/*   Updated: 2025/12/31 20:02:26 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseStatus.hpp"

Response checkMethodAllowed(Response &res, const LocationConfig &conf)
{
	std::stringstream	ss;

	res.statusCode = 405;
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	NotFound(Response &res)
{
	std::stringstream	ss;

	res.statusCode = 404;
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>404 Not Found</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	Forbidden(Response &res)
{
	std::stringstream	ss;

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

Response	DirListing(Response &res, std::string	ReqPath)
{
	std::stringstream	ss;
	DIR					*dir = opendir(res.fullPath.c_str());
	if (dir == NULL)
		return InternalServerError(res);
	struct dirent		*entry;

	ss << "<html><head><title>Index of " << ReqPath << "</title></head>";
	ss << "<body><h1>Index of " << ReqPath << "</h1>";
	ss << "<ul>";
	while (true)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;

		std::string	temp = entry->d_name;

		if (temp == "." || temp == "..")
			continue;

		if (isDirectory(res.fullPath + "/" + temp))
			temp += "/";
		ss << "<li><a href=\"" << temp << "\">" << temp << "</a></li>";
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

Response	InternalServerError(Response &res)
{
	std::stringstream	ss;

	res.statusCode = 500;
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	Conflict(Response &res)
{
	std::stringstream	ss;

	res.statusCode = 409;
	res.headers["Content-Type"] = "text/html";
	res.body = "<html><body><h1>409 Conflict - Cannot delete directory</h1></body></html>";
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

Response	BadRequest(Response &res, const std::string	&body)
{
	std::stringstream	ss;

	res.statusCode = 400;
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
