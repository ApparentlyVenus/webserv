/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:06:18 by odana             #+#    #+#             */
/*   Updated: 2025/12/26 19:13:33 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "ConfigFactory.hpp"
#include "FileUtils.hpp"
#include "Logger.hpp"

void printServerConfig(const ServerConfig& cfg) {
    std::cout << "\n=== SERVER CONFIG ===" << std::endl;
    
    std::vector<int> ports = cfg.getPorts();
    std::cout << "Ports: ";
    for (size_t i = 0; i < ports.size(); i++) {
        std::cout << ports[i];
        if (i < ports.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "IP: " << cfg.getIP() << std::endl;
    std::cout << "Server Name: " << cfg.getServerName() << std::endl;
    std::cout << "Root: " << cfg.getRoot() << std::endl;
    std::cout << "Max Body Size: " << cfg.getClientMaxBodySize() << " bytes" << std::endl;
    
    std::vector<LocationConfig> locations = cfg.getLocations();
    std::cout << "\n--- LOCATIONS (" << locations.size() << ") ---" << std::endl;
    
    for (size_t i = 0; i < locations.size(); i++) {
        std::cout << "\n[" << i << "] " << locations[i].getPath() << std::endl;
        
        std::vector<std::string> methods = locations[i].getAllowedMethods();
        std::cout << "  Methods: ";
        if (methods.empty()) {
            std::cout << "ALL";
        } else {
            for (size_t j = 0; j < methods.size(); j++) {
                std::cout << methods[j];
                if (j < methods.size() - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl;
        
        std::cout << "  Root: " << locations[i].getRoot() << std::endl;
        std::cout << "  Index: " << locations[i].getIndex() << std::endl;
        std::cout << "  AutoIndex: " << (locations[i].isAutoIndex() ? "on" : "off") << std::endl;
        
        if (locations[i].hasRedirect()) {
            std::cout << "  Redirect: " << locations[i].getRedirectCode() 
                      << " -> " << locations[i].getRedirect() << std::endl;
        }
        
        if (locations[i].isUploadEnable()) {
            std::cout << "  Upload: enabled (" << locations[i].getUploadStore() << ")" << std::endl;
        }
        
        std::vector<std::string> cgiExt = locations[i].getCgiExtensions();
        if (!cgiExt.empty()) {
            std::cout << "  CGI Extensions: ";
            for (size_t j = 0; j < cgiExt.size(); j++) {
                std::cout << cgiExt[j];
                if (j < cgiExt.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
            
            if (!locations[i].getCgiPy().empty())
                std::cout << "    Python: " << locations[i].getCgiPy() << std::endl;
            if (!locations[i].getCgiPhp().empty())
                std::cout << "    PHP: " << locations[i].getCgiPhp() << std::endl;
            if (!locations[i].getCgiPl().empty())
                std::cout << "    Perl: " << locations[i].getCgiPl() << std::endl;
        }
    }
    
    std::cout << "\n=====================\n" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];
    
    if (!fileExists(configPath)) {
        logError("Config file not found: " + configPath);
        return 1;
    }

    std::string content = readFile(configPath);
    if (content.empty()) {
        logError("Could not read config file");
        return 1;
    }

    try {
        Tokenizer tokenizer(content);
        std::vector<Token> tokens = tokenizer.tokenize();
        
        Parser parser(tokens);
        ServerBlock block = parser.parseServer();
        
        ServerConfig config = ConfigFactory::buildServer(block);
        
        printServerConfig(config);
        
        logInfo("Configuration loaded successfully");
        logError("yousef akal kaka");
        
    } catch (const std::exception& e) {
        logError(std::string("Error: ") + e.what());
        return 1;
    }

    return 0;
}