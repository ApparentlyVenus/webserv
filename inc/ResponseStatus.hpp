/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseStatus.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 19:11:54 by yitani            #+#    #+#             */
/*   Updated: 2026/01/07 11:23:02 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSESTATUS_HPP
# define RESPONSESTATUS_HPP

# include "Response.hpp"
# include "LocationConfig.hpp"
# include <dirent.h>
# include <sys/types.h>

Response    checkMethodAllowed(Response &res);
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