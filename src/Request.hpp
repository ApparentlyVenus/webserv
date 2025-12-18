/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:16:52 by odana             #+#    #+#             */
/*   Updated: 2025/12/18 22:19:25 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <cctype>
# include <map>
# include <vector>

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

#endif