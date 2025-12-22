/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:20:56 by odana             #+#    #+#             */
/*   Updated: 2025/12/22 10:36:40 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request Request::parse(const std::string& raw, size_t maxBodySize) {
    Request req;
    req.errorCode = 0;
    req.state = INCOMPLETE;

    size_t firstSep = raw.find("\r\n");
    if (firstSep == std::string::npos) {
        req.state = INCOMPLETE;
        return req;
    }
    
    std::string requestLineRaw = raw.substr(0, firstSep);
    std::vector<std::string> requestLineParsed = parseRequestLine(requestLineRaw);
    
    if (requestLineParsed.size() != 3) {
        req.state = ERROR;
        req.errorCode = 400;
        return req;
    }

    req.method = requestLineParsed[0];
    splitPathAndQuery(requestLineParsed[1], req.path, req.query);
    req.version = requestLineParsed[2];
    
    size_t headerSep = raw.find("\r\n\r\n");
    if (headerSep == std::string::npos) {
        req.state = INCOMPLETE;
        return req;
    }
    
    std::string headerRaw = raw.substr(firstSep + 2, headerSep - (firstSep + 2));
    req.headers = parseHeaders(headerRaw);

    req.body = raw.substr(headerSep + 4);
    if (req.headers.find("content-length") != req.headers.end()) {
        int contentLength = std::atoi(req.headers["content-length"].c_str());
        
        if (req.body.size() < static_cast<size_t>(contentLength)) {
            req.state = INCOMPLETE;
            return req;
        }
        req.body = req.body.substr(0, contentLength);
    }
    
    if (!isValid(req, maxBodySize)) {
        req.state = ERROR;
        return req;
    }

    req.state = COMPLETE;
    return req;
    
}

bool isValid(Request& req, size_t maxBodySize) {
    if (req.method != "GET" && req.method != "POST" && req.method != "DELETE") {
        req.errorCode = 501;
        return false;
    }
    
    if (req.version != "HTTP/1.1") {
        req.errorCode = 505;
        return false;
    }
    
    if (req.path.length() > 2048) {
        req.errorCode = 414;
        return false;
    }
    
    if (req.path.find("../") != std::string::npos) {
        req.errorCode = 400;
        return false;
    }
    
    if (req.headers.find("host") == req.headers.end()) {
        req.errorCode = 400;
        return false;
    }
    
    if (req.body.size() > maxBodySize) {
        req.errorCode = 413;
        return false;
    }
    
    return true;
}

std::vector<std::string> parseRequestLine(const std::string& raw) {
    std::vector<std::string> tokens;
    std::string token;
    
    for (size_t i = 0; i < raw.size(); i++) {
        if (std::isspace(raw[i])) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += raw[i];
        }
    }
    
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::map<std::string, std::string> parseHeaders(const std::string& raw) {
    std::map<std::string, std::string> headers;
    size_t pos = 0;
    
    while (pos < raw.size()) {
        size_t lineEnd = raw.find("\r\n", pos);
        if (lineEnd == std::string::npos) {
            lineEnd = raw.size();
        }
        
        std::string line = raw.substr(pos, lineEnd - pos);
        
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            key = trim(key);
            value = trim(value);
            key = toLower(key);
            
            headers[key] = value;
        }
        
        pos = lineEnd + 2;
    }
    
    return headers;
}

void splitPathAndQuery(const std::string& fullPath, std::string& path, std::string& query) {
    size_t queryPos = fullPath.find('?');
    
    if (queryPos != std::string::npos) {
        path = fullPath.substr(0, queryPos);
        query = fullPath.substr(queryPos + 1);
    } else {
        path = fullPath;
        query = "";
    }
}

std::string toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.size(); i++)
        result[i] = std::tolower(result[i]);
    return result;
}

std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.size();
    
    while (start < end && std::isspace(str[start]))
        start++;
    
    while (end > start && std::isspace(str[end - 1]))
        end--;
    
    return str.substr(start, end - start);
}