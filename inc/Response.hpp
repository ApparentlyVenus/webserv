/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:09 by yitani            #+#    #+#             */
/*   Updated: 2026/01/02 15:34:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include "Request.hpp"
# include "LocationConfig.hpp"
# include "FileUtils.hpp"
# include <cstdio>
# include <fstream>
# include "ServerConfig.hpp"

class Response
{
	private:
		std::string	serverName;
		int			serverPort;
	public:
		int									statusCode;
		std::string							fullPath;
		std::vector<std::string> 			setCookies;		
		std::map<std::string, std::string>	headers;
		std::string body;

		Response(Request &req, const LocationConfig &conf, const ServerConfig& servConf, int port);
		~Response();
		static std::string	format(Response &res);
		static std::string	getReasonPhrase(int statusCode);
		std::string			getServerName();
		std::string			getServerPort();
};

#endif