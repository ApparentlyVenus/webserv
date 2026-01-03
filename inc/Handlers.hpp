/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handlers.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 10:25:00 by yitani            #+#    #+#             */
/*   Updated: 2026/01/03 17:01:04 by odana            ###   ########.fr       */
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
		static Response handleDelete(Request &req, Response &res, const LocationConfig &conf);
		
		static Response handleDashboard(Request &req, Response &res, const LocationConfig &conf);
		static Response handleLogin(Request &req, Response &res, const LocationConfig &conf);
		static Response handleProfile(Request &req, Response &res, const LocationConfig &conf);
		static Response handleLogout(Request &req, Response &res, const LocationConfig &conf);

			
		static Response router(Response &res, Request &req, const LocationConfig &conf);

	private:
		Handlers();
		Handlers(const Handlers &other);
		Handlers &operator=(const Handlers &other);
		~Handlers();
};

#endif