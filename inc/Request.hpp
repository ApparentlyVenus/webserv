/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:57:31 by yitani            #+#    #+#             */
/*   Updated: 2025/12/25 21:55:17 by yitani           ###   ########.fr       */
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
		std::map<std::string, std::string> headers; // All headers: headers["host"] = "localhost:8080"
		std::string body;							// Request body (POST data, file uploads)
		parsingState state;							// COMPLETE, INCOMPLETE, or ERROR
		int errorCode;								// HTTP error code if state is ERROR (400, 413, etc.)

		static Request parse(const std::string &buffer, size_t maxBodySize);
};

#endif