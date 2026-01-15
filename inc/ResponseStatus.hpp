/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 19:11:54 by yitani            #+#    #+#             */
/*   Updated: 2026/01/15 19:01:43 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSESTATUS_HPP
# define RESPONSESTATUS_HPP

# include "Response.hpp"
# include "LocationConfig.hpp"
# include <dirent.h>
# include <sys/types.h>

Response	checkMethodAllowed(Response &res, const ServerConfig &servConf);
Response	NotFound(Response &res, const ServerConfig &servConf);
Response	Forbidden(Response &res, const ServerConfig &servConf);
Response	GetSuccess(Response &res, const std::string& filepath);
Response	DirListing(Response &res, std::string ReqPath, const ServerConfig &servConf);
Response	InternalServerError(Response &res, const ServerConfig &servConf);
Response	Conflict(Response &res, const ServerConfig &servConf);
Response	BadRequest(Response &res, const std::string	&body, const ServerConfig &servConf);
Response	PostSuccess(Response &res);
Response	DeleteSuccess(Response &res);

#endif