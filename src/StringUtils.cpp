/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:07:51 by yitani            #+#    #+#             */
/*   Updated: 2026/01/01 10:46:31 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/StringUtils.hpp"

bool toBool(const std::string &str)
{
	std::string lower = toLower(str);

	if (lower == "true" || lower == "on" || lower == "yes" || lower == "1")
		return true;
	if (lower == "false" || lower == "off" || lower == "no" || lower == "0")
		return true;
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

bool startsWith(const std::string &str, const std::string &prefix)
{
	size_t len;
	size_t i;

	len = prefix.length();
	i = 0;

	if (str.length() < len)
		return (false);

	while (i < len)
	{
		if (str[i] != prefix[i])
			return (false);
		i++;
	}
	return (true);
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
