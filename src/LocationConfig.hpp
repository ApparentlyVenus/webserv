/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 09:10:13 by odana             #+#    #+#             */
/*   Updated: 2025/12/22 10:17:00 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP 
# define LOCATION_CONFIG_HPP

# include <string>
# include <vector>

class ServerConfig;

class LocationConfig {
    private:
    
    std::string path;
    std::vector<std::string> allowedMethods;
    std::string root;
    bool autoIndex;
    std::string index;
    std::string redirect;
    int redirectCode;
    bool uploadEnable;
    std::string uploadStore;
    std::vector<std::string> cgiExtension;
    std::string cgiPath;

    public:
    
    std::string getPath() const;
    std::vector<std::string> getAllowedMethods() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getUploadStore() const;
    std::vector<std::string> getCgiExtensions() const;
    std::string getCgiPath() const;

    bool hasRedirect() const;
    std::string getRedirect() const;
    int getRedirectCode() const;

    bool isMethodAllowed(const std::string& method) const;
    bool isUploadEnable() const;
    bool isAutoIndex() const;
    bool isCGI(const std::string& path) const;
    
    std::string getTrueRoot(const ServerConfig& server) const;

    bool isValid() const;
};

#endif