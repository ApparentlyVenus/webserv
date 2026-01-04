/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handlers.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 10:25:00 by yitani            #+#    #+#             */
/*   Updated: 2026/01/05 01:09:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLERS_HPP
# define HANDLERS_HPP

# include "SessionManager.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "LocationConfig.hpp"
# include "FileUtils.hpp"
# include "StringUtils.hpp"
# include "ResponseStatus.hpp"
# include "ServerConfig.hpp"
# include "StringUtils.hpp"

# include <unistd.h>
# include <sys/wait.h>
# include <fstream>
# include <sstream>
# include <vector>

class Handlers
{
	public:
		static Response handleGet(Request &req, Response &res, const LocationConfig &conf);
		static Response handlePost(Request &req, Response &res, const LocationConfig &conf);
		static Response handleDelete(Response &res, const LocationConfig &conf);
			
		static Response router(Response &res, Request &req, const LocationConfig &conf);

	private:
		Handlers();
		Handlers(const Handlers &other);
		Handlers &operator=(const Handlers &other);
		~Handlers();
};

#endif