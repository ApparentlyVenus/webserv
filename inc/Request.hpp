/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:57:31 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 18:17:15 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>
# include "../inc/StringUtils.hpp"

enum parsingState
{
	COMPLETE,
	INCOMPLETE,
	ERROR
};

class Request
{
	public:
		Request();
		~Request();
		std::string method;
		std::string path;
		std::string query;
		std::string version;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> cookies;
		std::string body;
		parsingState state;
		int errorCode;

		static Request parse(const std::string &buffer, size_t maxBodySize);
};

#endif