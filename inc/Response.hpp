/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:52:09 by yitani            #+#    #+#             */
/*   Updated: 2025/12/26 22:19:09 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include "Request.hpp"
# include "LocationConfig.hpp"
# include "FileUtils.hpp"

class Response
{
	public:
		int statusCode;
		std::map<std::string, std::string> headers;
		std::string body;

		static std::string format(const Response &res);
		static std::string getReasonPhrase(int statusCode);
};

#endif