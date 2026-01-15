/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handlers.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 17:38:32 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 19:51:32 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Handlers.hpp"

static Session *loadSession(Request &req, Response &res, bool createIfMissing)
{
	(void)res;
	(void)createIfMissing;
	if (req.cookies.find("SESSID") != req.cookies.end())
	{
		Session *sess = SessionManager::getSession(req.cookies["SESSID"]);
		if (sess)
		{
			SessionManager::touchSession(sess->sessionId);
			return sess;
		}
	}
	return NULL;
}

static Response handleDashboard(Request &req, Response &res, const ServerConfig &servConf)
{
	Session* sess = loadSession(req, res, false);

	if (!sess || sess->data.find("logged_in") == sess->data.end())
	{
		res.statusCode = 302;
		res.headers["Location"] = "/test-login.html";
		res.headers["Content-Length"] = "0";
		res.body = "";
		return res;
	}

	std::string templatePath = res.getRootPath() + "/dashboard.html";
	std::string htmlTemplate = readFile(templatePath);

	if (htmlTemplate.empty())
		return InternalServerError(res, servConf);

	std::string html = replaceAll(htmlTemplate, "{{USERNAME}}", sess->data["username"]);
	html = replaceAll(html, "{{SESSION_ID}}", sess->sessionId);

	res.statusCode = 200;
	res.body = html;
	res.headers["Content-Type"] = "text/html";

	std::stringstream ss;
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();

	return res;
}

static Response handleProfile(Request &req, Response &res, const ServerConfig &servConf)
{
	Session* sess = loadSession(req, res, false);

	if (!sess || sess->data.find("logged_in") == sess->data.end())
	{
		res.statusCode = 302;
		res.headers["Location"] = "/test-login.html";
		res.headers["Content-Length"] = "0";
		res.body = "";
		return res;
	}

	int visits = 1;
	if (sess->data.find("visits") != sess->data.end())
	{
		visits = std::atoi(sess->data["visits"].c_str()) + 1;
	}

	std::stringstream vistStream;
	vistStream << visits;
	sess->data["visits"] = vistStream.str();

	std::string templatePath = res.getRootPath() + "/profile.html";
	std::string htmlTemplate = readFile(templatePath);

	if (htmlTemplate.empty())
		return InternalServerError(res, servConf);

	std::string html = replaceAll(htmlTemplate, "{{USERNAME}}", sess->data["username"]);
	html = replaceAll(html, "{{SESSION_ID}}", sess->sessionId);
	html = replaceAll(html, "{{VISITS}}", vistStream.str());

	res.statusCode = 200;
	res.body = html;
	res.headers["Content-Type"] = "text/html";

	std::stringstream ss;
	ss << res.body.length();
	res.headers["Content-Length"] = ss.str();

	return res;
}

static Response handleLogin(Request &req, Response &res, const ServerConfig &servConf)
{
	std::string username = "";
	size_t pos = req.body.find("username=");
	if (pos != std::string::npos)
	{
		size_t start = pos + 9;
		size_t end = req.body.find("&", start);
		if (end == std::string::npos)
			end = req.body.length();
		username = req.body.substr(start, end - start);
	}

	if (username.empty())
	{
		return BadRequest(res, "<html><body><h1>Username required</h1></body></html>", servConf);
	}

	std::string sessionId = SessionManager::createSession();
	Session *sess = SessionManager::getSession(sessionId);
	sess->data["username"] = username;
	sess->data["logged_in"] = "true";

	std::string cookie = "SESSID=" + sessionId + "; Path=/; Max-Age=3600; HttpOnly";
	res.setCookies.push_back(cookie);

	res.statusCode = 302;
	res.headers["Location"] = "/dashboard";
	res.headers["Content-Length"] = "0";
	res.body = "";

	return res;
}

static Response handleLogout(Request &req, Response &res)
{
	if (req.cookies.find("SESSID") != req.cookies.end())
	{
		SessionManager::destroySession(req.cookies["SESSID"]);
	}

	res.statusCode = 302;
	res.headers["Location"] = "/";
	res.headers["Content-Length"] = "0";
	res.body = "";

	return res;
}

static void	setUpPath(std::string &scriptName, std::string &pathInfo, const std::string &fullPath, const std::string &root)
{
	// fullPath is already the complete file path (e.g., www/cgi-bin/env.pl)
	// We need to extract the script name relative to root and any path info

	// Find where the actual script file is
	std::string testPath = fullPath;
	scriptName = "";
	pathInfo = "";

	// Check if fullPath itself is a file
	if (fileExists(testPath) && !isDirectory(testPath))
	{
		// Extract script name relative to root
		if (testPath.find(root) == 0)
		{
			scriptName = testPath.substr(root.length());
		}
		return;
	}

	// Try to find the script file by removing path segments from the end
	std::vector<std::string>	splitted = split(fullPath, '/');
	size_t						i = splitted.size();
	std::stringstream			pathSS;

	while (i > 0)
	{
		i--;
		testPath = "";
		for (size_t j = 0; j < i; j++)
		{
			if (!splitted[j].empty())
				testPath += "/" + splitted[j];
		}

		if (fileExists(testPath) && !isDirectory(testPath))
		{
			// Found the script file
			if (testPath.find(root) == 0)
			{
				scriptName = testPath.substr(root.length());
			}

			// Rest is path info
			for (size_t j = i; j < splitted.size(); j++)
			{
				if (!splitted[j].empty())
					pathSS << "/" << splitted[j];
			}
			pathInfo = pathSS.str();
			return;
		}
	}
}

static std::vector<std::string>	SetUpEnv(Request &req, Response &res)
{
	std::vector<std::string>						envStrings;
	std::map<std::string, std::string>::iterator	it;
	
	envStrings.push_back("REQUEST_METHOD=" + req.method);
	envStrings.push_back("QUERY_STRING=" + req.query);
	envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envStrings.push_back("SERVER_NAME=" + res.getServerName());
	envStrings.push_back("SERVER_PORT=" + res.getServerPort());
	envStrings.push_back("REMOTE_ADDR=" + res.getClientIP());
	envStrings.push_back("REDIRECT_STATUS=200");
	std::string	scriptName;
	std::string	pathInfo;
	setUpPath(scriptName, pathInfo, res.fullPath, res.getRootPath());
	envStrings.push_back("SCRIPT_NAME=" + scriptName);
	envStrings.push_back("PATH_INFO=" + pathInfo);
	envStrings.push_back("SCRIPT_FILENAME=" + res.fullPath);
	envStrings.push_back("PATH_TRANSLATED=" + res.getRootPath() + pathInfo);

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

static Response	handleCGI(Request &req, Response &res, const LocationConfig &conf, const ServerConfig &servConf)
{
	size_t dotPos = res.fullPath.find_last_of(".");
	std::string extension = res.fullPath.substr(dotPos);
	std::string interpreter = conf.getCgiInterpreter(extension);

	if (interpreter.empty())
		return (InternalServerError(res, servConf));
	
	std::vector<std::string> envStrings = SetUpEnv(req, res);
	char	**envArray = new char*[envStrings.size() + 1];
	for (size_t i = 0; i < envStrings.size(); i++)
	{
		envArray[i] = (char *)envStrings[i].c_str();
	}
	envArray[envStrings.size()] = NULL;
	
	int		pipe_from_child[2];
	int		pipe_to_child[2];
	if (pipe(pipe_from_child) == -1 || pipe(pipe_to_child) == -1)
	{
		delete[] (envArray);
		return (InternalServerError(res, servConf));
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		close(pipe_from_child[0]);
		close(pipe_from_child[1]);
		close(pipe_to_child[0]);
		close(pipe_to_child[1]);
		delete[] envArray;
		return InternalServerError(res, servConf);
	}

	if (pid == 0)
	{
		if (dup2(pipe_to_child[0], STDIN_FILENO) < 0)
		{
			close(pipe_to_child[0]);
			exit(1);
		}
		if (dup2(pipe_from_child[1], STDOUT_FILENO) < 0)
		{
			close(pipe_from_child[1]);
			exit(1);
		}
		close(pipe_to_child[0]);
		close(pipe_to_child[1]);
		close(pipe_from_child[0]);
		close(pipe_from_child[1]);
		char *argv[] = {(char *)interpreter.c_str(), (char *)res.fullPath.c_str(), NULL};
		execve(interpreter.c_str(), argv, envArray);
		exit(1);
	}
	
	else
	{
		close(pipe_from_child[1]);
		close(pipe_to_child[0]);
		if (req.method == "POST")
		{
			write(pipe_to_child[1], req.body.c_str(), req.body.size());
		}

		close(pipe_to_child[1]);

		char		buffer[4096];
		std::string	output;
		
		while (true)
		{
			ssize_t	bytes = read(pipe_from_child[0], buffer, sizeof(buffer));
			if (bytes <= 0)
				break ;
			output.append(buffer, bytes);
		}
		close(pipe_from_child[0]);

		int	status;
		waitpid(pid, &status, 0);
		delete[] (envArray);

		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			return InternalServerError(res, servConf);

		size_t	headersEnd = output.find("\r\n\r\n");
		size_t	headerLen = 4;

		if (headersEnd == std::string::npos)
		{
			headersEnd = output.find("\n\n");
			headerLen = 2;
		}

		if (headersEnd == std::string::npos)
			return InternalServerError(res, servConf);

		std::string	headers = output.substr(0, headersEnd);
		std::string	body = output.substr(headersEnd + headerLen);
		std::vector<std::string> splitHeaders = split(headers, '\n');
		for (size_t i = 0; i < splitHeaders.size(); i++)
		{
			size_t colon = splitHeaders[i].find(':');
			if (colon != std::string::npos)
			{
				std::string key = splitHeaders[i].substr(0, colon);
				std::string value = splitHeaders[i].substr(colon + 1);
				res.headers[trim(key)] = trim(value);
			}
		}

		res.body = body;
		if (res.headers.find("Content-Length") == res.headers.end())
		{
			std::stringstream ss;
			ss << body.length();
			res.headers["Content-Length"] = ss.str();
		}

		if (res.headers.find("Status") != res.headers.end())
		{
			res.statusCode = std::atoi(res.headers["Status"].c_str());
			res.headers.erase("Status");
		}
		else
		{
			if (req.method == "POST")
				res.statusCode = 201;
			else
				res.statusCode = 200;
		}

		return (res);
	}
}

Response Handlers::router(Response &res, Request &req, const LocationConfig &conf, const ServerConfig &servConf)
{
	if (req.method == "POST")
		return (handlePost(req, res, conf, servConf));

	else if (req.method == "GET")
		return (handleGet(req, res, conf, servConf));
	else if (req.method == "DELETE")
		return (handleDelete(res, conf, servConf));
	return (checkMethodAllowed(res, servConf));
}

Response Handlers::handleGet(Request &req, Response &res, const LocationConfig &conf, const ServerConfig &servConf)
{
	std::stringstream	ss;

	if (!conf.isMethodAllowed("GET"))
		return (checkMethodAllowed(res, servConf));

	if (req.path == "/dashboard")
		return handleDashboard(req, res, servConf);
	if (req.path == "/profile")
		return handleProfile(req, res, servConf);
	if (req.path == "/logout")
		return handleLogout(req, res);
		
	if (!fileExists(res.fullPath))
		return (NotFound(res, servConf));

	if (!isReadable(res.fullPath))
		return (Forbidden(res, servConf));

	if (isDirectory(res.fullPath))
	{
		std::string indexPath = res.fullPath;
		if (indexPath[indexPath.length() - 1] != '/')
			indexPath += "/";
		indexPath += conf.getIndex();

		if (fileExists(indexPath) && isReadable(indexPath))
			return (GetSuccess(res, indexPath));

		if (conf.isAutoIndex())
			return (DirListing(res, req.path, servConf));

		return (Forbidden(res, servConf));
	}

	if (conf.isCGI(res.fullPath))
		return handleCGI(req, res, conf, servConf);
	
	return (GetSuccess(res, res.fullPath));
}

Response Handlers::handlePost(Request &req, Response &res, const LocationConfig &conf, const ServerConfig &servConf)
{
	std::stringstream ss;

	if (!conf.isMethodAllowed("POST"))
		return (checkMethodAllowed(res, servConf));

	if (req.path == "/do_login")
		return handleLogin(req, res, servConf);

	if (conf.isCGI(res.fullPath))
		return handleCGI(req, res, conf, servConf);

	if (!conf.isUploadEnable())
		return (Forbidden(res, servConf));

	std::string uploadPath = conf.getUploadStore();
	if (!fileExists(uploadPath) || !isDirectory(uploadPath))
		return (InternalServerError(res, servConf));

	if (!isWritable(uploadPath))
		return (Forbidden(res, servConf));

	if (req.body.empty())
		return (BadRequest(res, "<html><body><h1>400 Bad Request - No data</h1></body></html>", servConf));

	std::string fileName;
	std::string fileContent;

	if (req.headers.find("content-type") != req.headers.end())
	{
		std::string contentType = req.headers["content-type"];
		if (contentType.find("multipart/form-data") != std::string::npos)
		{
			std::string boundary = extractBoundary(contentType);
			if (boundary.empty() || !parseMultipartFile(req.body, boundary, fileName, fileContent))
				return (BadRequest(res, "<html><body><h1>400 Bad Request - Invalid multipart data</h1></body></html>", servConf));
		}
		else
		{
			size_t pos = res.fullPath.find_last_of("/");
			fileName = res.fullPath.substr(pos + 1);
			fileContent = req.body;
		}
	}
	else
	{
		size_t pos = res.fullPath.find_last_of("/");
		fileName = res.fullPath.substr(pos + 1);
		fileContent = req.body;
	}

	if (fileName.empty() || fileName.find("..") != std::string::npos || fileName.length() > 255)
		return (BadRequest(res, "<html><body><h1>400 Bad Request - Invalid filename</h1></body></html>", servConf));

	std::string finalPath = uploadPath + "/" + fileName;
	std::ofstream file(finalPath.c_str(), std::ios::binary);
	if (!file.is_open())
		return (InternalServerError(res, servConf));

	file.write(fileContent.c_str(), fileContent.length());
	file.close();
	return (PostSuccess(res));
}

Response Handlers::handleDelete(Response &res, const LocationConfig &conf, const ServerConfig &servConf)
{
	if (!conf.isMethodAllowed("DELETE"))
		return (checkMethodAllowed(res, servConf));
		
	if (!fileExists(res.fullPath))
		return (NotFound(res, servConf));

	std::string dirPath = res.fullPath;

	if (isDirectory(dirPath))
		return (Conflict(res, servConf));

	size_t pos = dirPath.find_last_of("/");
	dirPath = res.fullPath.substr(0, pos);
	if (!isWritable(dirPath))
		return (Forbidden(res, servConf));

	if (std::remove(res.fullPath.c_str()) != 0)
		return (InternalServerError(res, servConf));

	return (DeleteSuccess(res));
}
