/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handlers.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 17:38:32 by yitani            #+#    #+#             */
/*   Updated: 2026/01/01 11:22:21 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Handlers.hpp"

static std::vector<std::string>	SetUpEnv(Request &req, Response &res)
{
	std::vector<std::string>						envStrings;
	std::map<std::string, std::string>::iterator	it;
	
	envStrings.push_back("REQUEST_METHOD=" + req.method);
	envStrings.push_back("QUERY_STRING=" + req.query);
	envStrings.push_back("SCRIPT_NAME=" + req.path);
	envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envStrings.push_back("SERVER_NAME=" + res.getServerName());
	envStrings.push_back("SERVER_PORT=" + res.getServerPort());

	for (it = req.headers.begin(); it != req.headers.end(); ++it)
	{
		std::string key = toUpper(it->first);
		for (size_t i = 0; i < key.size(); i++)
		{
			if (key[i] == '-')
				key[i] = '_';
		}
		std::string envKey = "HTTP_" + key;
		envStrings.push_back(envKey + "=" + it->second);
	}

	if (req.method == "POST")
	{
		std::stringstream ss;
		ss << req.body.length();
		envStrings.push_back("CONTENT_LENGTH=" + ss.str());

		if (req.headers.find("content-type") != req.headers.end())
			envStrings.push_back("CONTENT_TYPE=" + req.headers["content-type"]);
	}

	return (envStrings);
}

// be2tol hale aw kif??
static Response	handleCGI(Request &req, Response &res, const LocationConfig &conf)
{}
// {
// 	int		pipefd[2];
// 	if (pipe(pipefd) == -1)
// 	{
// 		// error 
// 	}

// 	pid_t	pid = fork();
// 	if (pid == -1)
// 	{
// 		// error
// 	}
// 	else if (pid == 0)
// 	{
// 		close(pipefd[0]);
// 		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
// 		{
// 			exit(1);
// 		}
// 		close(pipefd[1]);
// 		execve(interpreter, res.fullPath, env);
// 	}


// 	if (req.method == "POST")
// 	{

// 	}

// 	if (req.method == "GET")
// 	{

// 	}

// 	return (res);
// }

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

	if (conf.isCGI(res.fullPath))
		return handleCGI(req, res, conf);
	
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

	if (conf.isCGI(res.fullPath))
		return handleCGI(req, res, conf);
		
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
