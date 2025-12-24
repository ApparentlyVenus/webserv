/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 18:29:34 by yitani            #+#    #+#             */
/*   Updated: 2025/12/24 18:33:05 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <iostream>
# include <ctime>
# include <time.h>

void	logError(const std::string &message);
void	logRequest(const std::string &method, const std::string &path, int statusCode);
void	logInfo(const std::string &message);

#endif