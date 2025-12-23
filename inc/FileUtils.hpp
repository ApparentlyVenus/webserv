/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:34:26 by yitani            #+#    #+#             */
/*   Updated: 2025/12/23 14:19:46 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

# include "StringUtils.hpp"
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>

bool		fileExists(const std::string& path);              // stat() wrapper
bool		isReadable(const std::string& path);              // access() wrapper
ssize_t		getFileSize(const std::string& path);           // stat() wrapper
std::string	readFile(const std::string& path);         // ifstream wrapper
bool		isDirectory(const std::string& path);             // stat() wrapper

#endif