#include <iostream>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "FileUtils.hpp"

void printDirective(const Directive& dir) {
    std::cout << "  " << dir.key.val << " ";
    for (size_t i = 0; i < dir.values.size(); i++) {
        std::cout << dir.values[i].val;
        if (i < dir.values.size() - 1)
            std::cout << " ";
    }
    std::cout << ";" << std::endl;
}

void printLocation(const LocationBlock& loc) {
    std::cout << "  location " << loc.path.val << " {" << std::endl;
    for (size_t i = 0; i < loc.directives.size(); i++) {
        std::cout << "  ";
        printDirective(loc.directives[i]);
    }
    std::cout << "  }" << std::endl;
}

void printServer(const ServerBlock& srv) {
    std::cout << "server {" << std::endl;
    for (size_t i = 0; i < srv.directives.size(); i++) {
        printDirective(srv.directives[i]);
    }
    for (size_t i = 0; i < srv.locations.size(); i++) {
        printLocation(srv.locations[i]);
    }
    std::cout << "}" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::string configPath = argv[1];
    
    if (!fileExists(configPath)) {
        std::cerr << "Error: Config file not found: " << configPath << std::endl;
        return 1;
    }

    std::string content = readFile(configPath);
    if (content.empty()) {
        std::cerr << "Error: Could not read config file" << std::endl;
        return 1;
    }

    try {
        Tokenizer tokenizer(content);
        std::vector<Token> tokens = tokenizer.tokenize();

        std::cout << "=== TOKENS ===" << std::endl;
        for (size_t i = 0; i < tokens.size(); i++) {
            std::cout << "Type: " << tokens[i].type << " Val: '" << tokens[i].val << "'" << std::endl;
        }
        std::cout << std::endl;

        Parser parser(tokens);
        ServerBlock server = parser.parseServer();

        std::cout << "=== PARSED CONFIG ===" << std::endl;
        printServer(server);

    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}