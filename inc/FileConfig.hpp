/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 23:59:28 by odana             #+#    #+#             */
/*   Updated: 2026/01/18 00:09:21 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_CONFIG_HPP
# define FILE_CONFIG_HPP

# include <vector>
# include <set>
# include "ServerConfig.hpp"

class FileConfig
{
    private:
        std::vector<ServerConfig> servers;
    
    public:
        FileConfig();
        FileConfig(const FileConfig& other);
        FileConfig& operator=(const FileConfig& other);
        ~FileConfig();
    
        void addServer(const ServerConfig& server);
        std::vector<ServerConfig> getServers() const;
        size_t getServerCount() const;
        bool isValid() const;
        
    private:
        bool hasConflictingBindings() const;
        std::set<std::string> getAllBindings() const;
};

#endif