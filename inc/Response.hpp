/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:09 by yitani            #+#    #+#             */
/*   Updated: 2025/12/29 21:31:15 by yitani           ###   ########.fr       */
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

class Response
{
	public:
		int									statusCode;
		std::string							fullPath;
		std::map<std::string, std::string>	headers;
		std::string body;

		Response			createResponse(Request &req, const LocationConfig &conf);
		static std::string	format(Response &res);
		static std::string	getReasonPhrase(int statusCode);
};

#endif