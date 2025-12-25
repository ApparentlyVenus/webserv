#ifndef CONFIG_FACTORY_HPP
# define CONFIG_FACTORY_HPP

# include "LocationConfig.hpp"
# include "ServerConfig.hpp"
# include "Parser.hpp"
# include "StringUtils.hpp"
# include <string>
# include <sstream>

class ConfigFactory {
public:
    static ServerConfig buildServer(const ServerBlock& block);
    static LocationConfig buildLocation(const LocationBlock& block);
    
private:
    ConfigFactory();
    ConfigFactory(const ConfigFactory& other);
    ConfigFactory& operator=(const ConfigFactory& other);
    ~ConfigFactory();

    static void processServerDirective(ServerConfig& cfg, const Directive& dir);
    static void processLocationDirective(LocationConfig& cfg, const Directive& dir);
    
    static void validateServerConfig(const ServerConfig& cfg);
    static void validateLocationConfig(const LocationConfig& cfg);
};

#endif