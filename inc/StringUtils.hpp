/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:06:11 by yitani            #+#    #+#             */
/*   Updated: 2026/01/01 10:48:45 by yitani           ###   ########.fr       */
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
bool						startsWith(const std::string& str, const std::string& prefix);
bool                        toBool(const std::string& str);
int                         toInt(const std::string& str);
#endif