/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:06:11 by yitani            #+#    #+#             */
/*   Updated: 2026/01/03 16:26:05 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

# include <string>
# include <cstdlib>
# include <stdexcept>
# include <sstream>
# include <vector>

std::string					trim(const std::string& str);
std::vector<std::string>	split(const std::string& str, char delim);
std::string					toLower(const std::string& str);
std::string					toUpper(const std::string &str);
bool                        toBool(const std::string& str);
int                         toInt(const std::string& str);
std::string                 replaceAll(const std::string& str, const std::string& find, const std::string& replace);
std::string					extractBoundary(const std::string &contentType);
bool						parseMultipartFile(const std::string &body, const std::string &boundary, std::string &filename, std::string &fileContent);

#endif