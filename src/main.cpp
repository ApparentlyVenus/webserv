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
			logError("Invalid server configuration");
			return 1;
		}

		// this needs fixing now for each port and like whatever wael has to do
		std::vector<int> ports = config.getPorts();
		std::stringstream portsMsg;
		portsMsg << "Loaded server config: " << config.getServerName() << " on ports: ";
		for (size_t i = 0; i < ports.size(); i++)
		{
			portsMsg << ports[i];
			if (i < ports.size() - 1)
				portsMsg << ", ";
		}
		logInfo(portsMsg.str());

		Server server(config);
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
