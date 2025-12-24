/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/12/24 18:05:36 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/FileUtils.hpp"

bool	fileExists(const std::string& path)
{
	struct stat buff;

	if (stat(path.c_str(), &buff) != 0)
		return (false);
	return (true);
}

ssize_t	getFileSize(const std::string& path)
{
	struct stat buff;

	if (stat(path.c_str(), &buff) == 0)
		return (buff.st_size);
	return (-1);
}

bool	isDirectory(const std::string& path)
{
	struct stat buff;

	if (stat(path.c_str(), &buff) != 0)
		return (false);
	return (S_ISDIR(buff.st_mode));
}

bool	isReadable(const std::string& path)
{
	if (access(path.c_str(), R_OK) != 0)
		return (false);
	return (true);
}

std::string readFile(const std::string& path)
{
	std::string content;
	char c;

	if (!fileExists(path) || !isReadable(path))
		return "";

	std::ifstream f(path.c_str(), std::ios::binary);
	if (!f.is_open())
		return "";

	while (f.get(c))
		content += c;

	return (content);
}

std::string getMimeType(const std::string &path)
{
	size_t dot = path.find_last_of('.');
	if (dot == std::string::npos)
		return ("application/octet-stream");

	std::string ext = path.substr(dot);

	if (ext == ".html" || ext == ".htm")
		return ("text/html");
	if (ext == ".css")
		return ("text/css");
	if (ext == ".js")
		return ("application/javascript");
	if (ext == ".json")
		return ("application/json");
	if (ext == ".png")
		return ("image/png");
	if (ext == ".jpg" || ext == ".jpeg")
		return ("image/jpeg");
	if (ext == ".gif")
		return ("image/gif");
	if (ext == ".svg")
		return ("image/svg+xml");
	if (ext == ".ico")
		return ("image/x-icon");
	if (ext == ".txt")
		return ("text/plain");
	if (ext == ".pdf")
		return ("application/pdf");
	if (ext == ".xml")
		return ("application/xml");

	return ("application/octet-stream");
}
