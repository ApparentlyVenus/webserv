/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:06:18 by odana             #+#    #+#             */
/*   Updated: 2026/01/18 00:17:26 by odana            ###   ########.fr       */
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
#include <csignal>

static Server *g_server = NULL;

void signalHandler(int signum)
{
	(void)signum;
	logInfo("\nReceived signal, shutting down server...");
	if (g_server)
		g_server->stop();
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		logError("Usage: ./webserv <config_file>");
		return 1;
	}

	std::string configPath = argv[1];

	if (!fileExists(configPath))
	{
		logError("Config file not found: " + configPath);
		return 1;
	}

	std::string content = readFile(configPath);
	if (content.empty())
	{
		logError("Could not read config file");
		return 1;
	}

	try
	{
	    logInfo("Parsing configuration file: " + configPath);

	    Tokenizer tokenizer(content);
	    std::vector<Token> tokens = tokenizer.tokenize();

	    Parser parser(tokens);
	    std::vector<ServerBlock> blocks = parser.parseServers();
	    FileConfig fileConfig = ConfigFactory::buildFileConfig(blocks);

	    if (!fileConfig.isValid())
	    {
	        logError("Invalid file configuration");
	        return 1;
	    }

	    // Log all server configs
	    std::vector<ServerConfig> configs = fileConfig.getServers();
	    std::stringstream configMsg;
	    configMsg << "Loaded " << configs.size() << " server configuration(s):";
	    logInfo(configMsg.str());
	
	    for (size_t i = 0; i < configs.size(); i++)
	    {
	        std::vector<int> ports = configs[i].getPorts();
	        std::stringstream serverMsg;
	        serverMsg << "  [" << i << "] " << configs[i].getServerName() 
	                  << " on " << configs[i].getIP() << " ports: ";
	        for (size_t j = 0; j < ports.size(); j++)
	        {
	            serverMsg << ports[j];
	            if (j < ports.size() - 1)
	                serverMsg << ", ";
	        }
	        logInfo(serverMsg.str());
	    }

	    Server server(fileConfig);
	    g_server = &server;

	    signal(SIGINT, signalHandler);
	    signal(SIGTERM, signalHandler);

	    logInfo("========================================");
	    logInfo("Starting web server...");
	    logInfo("========================================");

	    server.run();

	    logInfo("Server stopped gracefully");
	    g_server = NULL;
	}
	catch (const std::exception &e)
	{
		logError(std::string("Fatal error: ") + e.what());
		g_server = NULL;
		return 1;
	}

	return 0;
}
