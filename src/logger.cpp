/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 18:29:52 by yitani            #+#    #+#             */
/*   Updated: 2025/12/24 18:35:07 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/logger.hpp"

static std::string	getCurrentTimestamp()
{
	time_t now = time(0);
	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
	return std::string(buf);
}

void	logError(const std::string &message)
{
	std::cerr << "[" << getCurrentTimestamp() << "] [ERROR] "
			  << message << std::endl;
}

void	logRequest(const std::string &method, const std::string &path, int statusCode)
{
	std::cout << "[" << getCurrentTimestamp() << "] "
			  << method << " " << path << " -> " << statusCode
			  << std::endl;
}

void	logInfo(const std::string &message)
{
	std::cout << "[" << getCurrentTimestamp() << "] [INFO] "
			  << message << std::endl;
}
