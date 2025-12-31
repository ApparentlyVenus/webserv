/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handlers.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 17:38:32 by yitani            #+#    #+#             */
/*   Updated: 2025/12/31 19:52:40 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Handlers.hpp"

Response Handlers::router(Response &res, Request &req, const LocationConfig &conf)
{
	if (req.method == "POST")
		return (handlePost(req, res, conf));

	else if (req.method == "GET")
		return (handleGet(req, res, conf));

	return (handleDelete(req, res, conf));
}

Response Handlers::handleGet(Request &req, Response &res, const LocationConfig &conf)
{
	std::stringstream	ss;

	if (!conf.isMethodAllowed("GET"))
		return (checkMethodAllowed(res, conf));

	if (!fileExists(res.fullPath))
		return (NotFound(res));

	if (!isReadable(res.fullPath))
		return (Forbidden(res));

	if (isDirectory(res.fullPath))
	{
		std::string indexPath = res.fullPath;
		if (indexPath[indexPath.length() - 1] != '/')
			indexPath += "/";
		indexPath += conf.getIndex();

		if (fileExists(indexPath) && isReadable(indexPath))
			return (GetSuccess(res, indexPath));

		if (conf.isAutoIndex())
			return (DirListing(res, req.path));

		return (Forbidden(res));
	}

	return (GetSuccess(res, res.fullPath));
}

Response Handlers::handlePost(Request &req, Response &res, const LocationConfig &conf)
{
	std::stringstream ss;
	std::string uploadPath;
	std::string fileName;

	uploadPath = conf.getUploadStore();
	size_t pos = res.fullPath.find_last_of("/");
	fileName = res.fullPath.substr(pos + 1);

	if (!conf.isMethodAllowed("POST"))
		return (checkMethodAllowed(res, conf));

	if (!conf.isUploadEnable())
		return (Forbidden(res));

	if (!fileExists(uploadPath) || !isDirectory(uploadPath))
		return (InternalServerError(res));

	if (!isWritable(uploadPath))
		return (Forbidden(res));

	if (req.body.empty())
		return (BadRequest(res, "<html><body><h1>400 Bad Request - No data</h1></body></html>"));

	if (fileName.empty() || fileName.find("..") != std::string::npos || fileName.length() > 255)
		return (BadRequest(res, "<html><body><h1>400 Bad Request - Invalid filename</h1></body></html>"));

	std::string finalPath = uploadPath + "/" + fileName;
	std::ofstream file(finalPath.c_str(), std::ios::binary);
	if (!file.is_open())
		return (InternalServerError(res));

	file.write(req.body.c_str(), req.body.length());
	file.close();
	return (PostSuccess(res));
}

Response Handlers::handleDelete(Request &req, Response &res, const LocationConfig &conf)
{
	if (!conf.isMethodAllowed("DELETE"))
		return (checkMethodAllowed(res, conf));
		
	if (!fileExists(res.fullPath))
		return (NotFound(res));

	std::string dirPath = res.fullPath;

	if (isDirectory(dirPath))
		return (Conflict(res));

	size_t pos = dirPath.find_last_of("/");
	dirPath = res.fullPath.substr(0, pos);
	if (!isWritable(dirPath))
		return (Forbidden(res));

	if (std::remove(res.fullPath.c_str()) != 0)
		return (InternalServerError(res));

	return (DeleteSuccess(res));
}
