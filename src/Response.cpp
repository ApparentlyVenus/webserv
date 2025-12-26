/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:35 by yitani            #+#    #+#             */
/*   Updated: 2025/12/26 22:49:22 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

static Response	handleGet(Request &req, LocationConfig &conf)
{
	std::string fullPath = conf.getRoot() + req.path;
	Response res;

	if (!fileExists(fullPath))
	{
		res.statusCode = 404;
		res.body = "<html><body><h1>404 Not Found</h1></body></html>";
		return res;
	}

	if (!isReadable(fullPath))
	{
		res.statusCode = 403;
		res.body = "<html><body><h1>403 Forbidden</h1></body></html>";
		return res;
	}

	res.statusCode = 200;
	res.body = readFile(fullPath);
	return res;
}

static std::string	format(const Response &res)
{
	
}