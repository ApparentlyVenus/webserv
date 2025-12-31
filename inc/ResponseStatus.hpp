/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 19:11:54 by yitani            #+#    #+#             */
/*   Updated: 2025/12/31 19:53:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSESTATUS_HPP
# define RESPONSESTATUS_HPP

# include "Response.hpp"
# include "LocationConfig.hpp"
# include <dirent.h>
# include <sys/types.h>

Response checkMethodAllowed(Response &res, const LocationConfig &conf);
Response	NotFound(Response &res);
Response	Forbidden(Response &res);
Response	GetSuccess(Response &res, const std::string& filepath);
Response	DirListing(Response &res, std::string ReqPath);
Response	InternalServerError(Response &res);
Response	Conflict(Response &res);
Response	BadRequest(Response &res, const std::string	&body);
Response	PostSuccess(Response &res);
Response	DeleteSuccess(Response &res);

#endif