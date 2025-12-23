/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:16:52 by odana             #+#    #+#             */
/*   Updated: 2025/12/23 13:30:22 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <cstdlib>
# include <map>
# include <vector>
# include "../inc/StringUtils.hpp"

enum parsingState {
    COMPLETE,
    INCOMPLETE,
    ERROR,
};

struct Request {
    std::string method;
    std::string version;
    std::string path;
    std::string query;
    std::string body;
    std::map<std::string, std::string> headers;
    parsingState state;
    int errorCode;

    static Request parse(const std::string& raw, size_t maxBodySize);
};

static std::vector<std::string> parseRequestLine(const std::string& raw);
static std::map<std::string, std::string> parseHeaders(const std::string& raw);
static void splitPathAndQuery(const std::string& fullPath, std::string& path, std::string& query);
static std::string toLower(const std::string& str);
static std::string trim(const std::string& str);
static bool isValid(Request& req, size_t maxBodySize);

#endif