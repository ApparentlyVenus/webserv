/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:09 by yitani            #+#    #+#             */
/*   Updated: 2026/01/03 16:39:45 by odana            ###   ########.fr       */
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
		std::string	clientIP;
		std::string rootPath;
		
	public:
		int									statusCode;
		std::string							fullPath;
		std::vector<std::string> 			setCookies;		
		std::map<std::string, std::string>	headers;
		std::string body;

		Response(Request &req, const LocationConfig &conf, const ServerConfig& servConf, int port, std::string IP);
		~Response();
		static std::string	format(Response &res);
		static std::string	getReasonPhrase(int statusCode);
		std::string			getServerName();
		std::string			getServerPort();
		std::string			getClientIP();
		std::string			getRootPath();
};

#endif