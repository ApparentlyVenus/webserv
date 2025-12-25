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
    
    LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    ~LocationConfig();

    std::string getPath() const;
    std::vector<std::string> getAllowedMethods() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getUploadStore() const;
    std::vector<std::string> getCgiExtensions() const;
    std::string getCgiPath() const;

    void setPath(const std::string& path);
    void addAllowedMethod(const std::string& method);
    void setRoot(const std::string& root);
    void setAutoIndex(bool enable);
    void setIndex(const std::string& idx);
    void setRedirect(const std::string& url, int code);
    void setUploadEnable(bool enable);
    void setUploadStore(const std::string& store);
    void addCgiExtension(const std::string& ext);
    void setCgiPath(const std::string& path);

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