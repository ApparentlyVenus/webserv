/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:34:26 by yitani            #+#    #+#             */
/*   Updated: 2025/12/26 22:21:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

# include "StringUtils.hpp"
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>

bool		fileExists(const std::string& path);
bool		isReadable(const std::string& path);
ssize_t		getFileSize(const std::string& path);
std::string	readFile(const std::string& path);
bool		isDirectory(const std::string& path);

std::string	getMimeType(const std::string& path);

#endif