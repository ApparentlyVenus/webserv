/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:07:51 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 19:51:36 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/StringUtils.hpp"

bool toBool(const std::string &str)
{
	std::string lower = toLower(str);

	if (lower == "true" || lower == "on" || lower == "yes" || lower == "1")
		return true;
	if (lower == "false" || lower == "off" || lower == "no" || lower == "0")
		return false;
	throw std::runtime_error("invalid boolean " + str);
}

int toInt(const std::string &str)
{
	if (str.empty())
		throw std::runtime_error("cannot convert empty string");

	int num = 0;
	std::stringstream ss(str);

	if (!(ss >> num) || !ss.eof())
		throw std::runtime_error("invalid interger " + str);
	return num;
}

std::string toUpper(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < result.size(); i++)
		result[i] = std::toupper(result[i]);
	return (result);
}

std::string toLower(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < result.size(); i++)
		result[i] = std::tolower(result[i]);
	return result;
}

std::string trim(const std::string &str)
{
	size_t start = 0;
	size_t end = str.size();

	while (start < end && std::isspace(str[start]))
		start++;

	while (end > start && std::isspace(str[end - 1]))
		end--;

	return str.substr(start, end - start);
}

std::string extractBoundary(const std::string &contentType)
{
	size_t pos = contentType.find("boundary=");
	if (pos == std::string::npos)
		return "";

	std::string boundary = contentType.substr(pos + 9);
	if (!boundary.empty() && boundary[0] == '"')
	{
		boundary = boundary.substr(1);
		size_t endQuote = boundary.find('"');
		if (endQuote != std::string::npos)
			boundary = boundary.substr(0, endQuote);
	}
	return boundary;
}

bool parseMultipartFile(const std::string &body, const std::string &boundary, std::string &filename, std::string &fileContent)
{
	std::string delimiter = "--" + boundary;
	size_t start = body.find(delimiter);
	if (start == std::string::npos)
		return false;

	start += delimiter.length();
	size_t headersEnd = body.find("\r\n\r\n", start);
	if (headersEnd == std::string::npos)
		headersEnd = body.find("\n\n", start);
	if (headersEnd == std::string::npos)
		return false;

	std::string headers = body.substr(start, headersEnd - start);
	size_t filenamePos = headers.find("filename=\"");
	if (filenamePos == std::string::npos)
		return false;

	filenamePos += 10;
	size_t filenameEnd = headers.find("\"", filenamePos);
	if (filenameEnd == std::string::npos)
		return false;

	filename = headers.substr(filenamePos, filenameEnd - filenamePos);

	size_t contentStart = headersEnd + 4;
	if (body.substr(headersEnd, 2) == "\n\n")
		contentStart = headersEnd + 2;

	size_t contentEnd = body.find(delimiter, contentStart);
	if (contentEnd == std::string::npos)
		return false;

	while (contentEnd > contentStart && (body[contentEnd - 1] == '\r' || body[contentEnd - 1] == '\n'))
		contentEnd--;

	fileContent = body.substr(contentStart, contentEnd - contentStart);
	return true;
}

std::vector<std::string> split(const std::string &str, char delim)
{
	std::vector<std::string> container;
	size_t pos = 0;
	size_t start = 0;

	while (pos != std::string::npos)
	{
		pos = str.find(delim, start);
		if (pos != std::string::npos)
		{
			container.push_back(str.substr(start, pos - start));
			start = pos + 1;
		}
		else
			container.push_back(str.substr(start));
	}
	return (container);
}

std::string replaceAll(const std::string& str, const std::string& find, const std::string& replace) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(find, pos)) != std::string::npos) {
        result.replace(pos, find.length(), replace);
        pos += replace.length();
    }
    
    return result;
}
