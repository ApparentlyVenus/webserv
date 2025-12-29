/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by yitani            #+#    #+#             */
/*   Updated: 2025/12/29 13:44:44 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response	Response::createResponse(Request &req, const LocationConfig &conf)
{
	Response	res;

	res.fullPath = conf.getRoot() + req.path;
	if (req.headers.find("connection") != req.headers.end())
		res.headers["Connection"] = req.headers["connection"];
	else
		res.headers["Connection"] = "close";


	if (req.method == "POST")
		return (handlePost(req, res));

	else if (req.method == "GET")
		return (handleGet(req, res));

	return (handleDelete(req, res));
}

static Response	handleGet(Request &req, Response &res)
{

	if (!fileExists(res.fullPath))
	{
		res.statusCode = 404;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>404 Not Found</h1></body></html>";
		res.headers["Content-Length"] = res.body.length();
		return res;
	}

	if (!isReadable(res.fullPath))
	{
		res.statusCode = 403;
		res.headers["Content-Type"] = "text/html";
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		res.headers["Content-Length"] = res.body.length();
		return res;
	}

	std::stringstream	ss;

	res.statusCode = 200;
	res.body = readFile(res.fullPath);
	res.headers["Content-Type"] = getMimeType(res.fullPath);
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();
	return (res);
}

static Response	handlePost(Request &req, Response &res)
{
	
}

static Response	handleDelete(Request &req, Response &res)
{
	
}

static std::string	format(const Response &res)
{
}