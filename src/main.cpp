/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:06:18 by odana             #+#    #+#             */
/*   Updated: 2026/01/02 22:03:40 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <csignal>
#include "../inc/Tokenizer.hpp"
#include "../inc/Parser.hpp"
#include "../inc/ConfigFactory.hpp"
#include "../inc/FileUtils.hpp"
#include "../inc/Logger.hpp"
#include "../inc/Server.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <csignal>


// Global server pointer for signal handling (MUST be at the top, before signalHandler)
Server *g_server = NULL;

// Signal handler function (MUST be declared before main)
void signalHandler(int signum)
{
    std::cout << "\nInterrupt signal (" << signum << ") received." << std::endl;
    if (g_server)
    {
        g_server->stop();
    }
}

bool fileExists1(const std::string& path)
{
    std::ifstream file(path.c_str());
    return file.good();
}

std::string readFile2(const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return "";
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void logError3(const std::string& message)
{
    std::cerr << "Error: " << message << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];

    if (!fileExists1(configPath))
    {
        logError3("Config file not found: " + configPath);
        return 1;
    }

    std::string content = readFile2(configPath);
    if (content.empty())
    {
        logError3("Could not read config file");
        return 1;
    }

    try
    {
        // Parse configuration file
        std::cout << "Parsing configuration file: " << configPath << std::endl;
        
        Tokenizer tokenizer(content);
        std::vector<Token> tokens = tokenizer.tokenize();

        Parser parser(tokens);
        
        // Parse all server blocks
        std::vector<ServerConfig> configs;
        
        // Try to parse multiple server blocks if present
        while (!parser.isAtEnd())
        {
            try
            {
                ServerBlock block = parser.parseServer();
                ServerConfig config = ConfigFactory::buildServer(block);
                
                // Validate configuration
                if (!config.isValid())
                {
                    logError3("Invalid server configuration found");
                    continue;
                }
                
                configs.push_back(config);
                std::cout << "Loaded server config: " << config.getServerName() 
                          << " on ports: ";
                std::vector<int> ports = config.getPorts();
                for (size_t i = 0; i < ports.size(); i++)
                {
                    std::cout << ports[i];
                    if (i < ports.size() - 1)
                        std::cout << ", ";
                }
                std::cout << std::endl;
            }
            catch (const std::exception& e)
            {
                // If we already have configs, this might just be end of file
                if (configs.empty())
                {
                    logError3(std::string("Failed to parse server block: ") + e.what());
                    return 1;
                }
                break;
            }
        }
        
        if (configs.empty())
        {
            logError3("No valid server configurations found");
            return 1;
        }

        std::cout << "Successfully loaded " << configs.size() << " server configuration(s)" << std::endl;
        
        // Create and run server
        Server server(configs);
        g_server = &server;  // Now this will work
        
        // Setup signal handlers for graceful shutdown
        signal(SIGINT, signalHandler);   // Now signalHandler is declared
        signal(SIGTERM, signalHandler);
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Starting web server..." << std::endl;
        std::cout << "Press Ctrl+C to stop" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        // Run the server (blocking call)
        server.run();
        
        std::cout << "Server stopped gracefully" << std::endl;
    }
    catch (const std::exception& e)
    {
        logError3(std::string("Fatal error: ") + e.what());
        return 1;
    }

    return 0;
}