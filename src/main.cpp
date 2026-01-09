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

        // Parse single server block
        ServerBlock block = parser.parseServer();
        ServerConfig config = ConfigFactory::buildServer(block);

        // Validate configuration
        if (!config.isValid())
        {
            logError3("Invalid server configuration");
            return 1;
        }

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

        std::cout << "Successfully loaded server configuration" << std::endl;

        // Create and run server
        Server server(config);

        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Starting web server..." << std::endl;
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