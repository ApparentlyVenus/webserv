/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:57:31 by yitani            #+#    #+#             */
/*   Updated: 2026/01/02 21:53:12 by wasmar           ###   ########.fr       */
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