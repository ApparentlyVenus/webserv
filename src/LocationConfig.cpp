#include "LocationConfig.hpp"
#include "ServerConfig.hpp"

LocationConfig::LocationConfig() 
    : path(""), root(""), autoIndex(false), index("index.html"),
      redirect(""), redirectCode(0), uploadEnable(false), 
      uploadStore(""), cgiPath("") {}

LocationConfig::LocationConfig(const LocationConfig& other)
    : path(other.path), allowedMethods(other.allowedMethods),
      root(other.root), autoIndex(other.autoIndex), index(other.index),
      redirect(other.redirect), redirectCode(other.redirectCode),
      uploadEnable(other.uploadEnable), uploadStore(other.uploadStore),
      cgiExtension(other.cgiExtension), cgiPath(other.cgiPath) {}

LocationConfig& LocationConfig::operator=(const LocationConfig& other) {
    if (this != &other) {
        path = other.path;
        allowedMethods = other.allowedMethods;
        root = other.root;
        autoIndex = other.autoIndex;
        index = other.index;
        redirect = other.redirect;
        redirectCode = other.redirectCode;
        uploadEnable = other.uploadEnable;
        uploadStore = other.uploadStore;
        cgiExtension = other.cgiExtension;
        cgiPath = other.cgiPath;
    }
    return *this;
}

LocationConfig::~LocationConfig() {}


std::string LocationConfig::getPath() const {
    return path;
}

std::vector<std::string> LocationConfig::getAllowedMethods() const {
    return allowedMethods;
}

std::string LocationConfig::getRoot() const {
    return root;
}

std::string LocationConfig::getIndex() const {
    return index;
}

std::string LocationConfig::getUploadStore() const {
    return uploadStore;
}

std::vector<std::string> LocationConfig::getCgiExtensions() const {
    return cgiExtension;
}

std::string LocationConfig::getCgiPath() const {
    return cgiPath;
}

bool LocationConfig::hasRedirect() const {
    return !redirect.empty();
}

std::string LocationConfig::getRedirect() const {
    return redirect;
}

int LocationConfig::getRedirectCode() const {
    return redirectCode;
}

bool LocationConfig::isUploadEnable() const {
    return uploadEnable;
}

bool LocationConfig::isAutoIndex() const {
    return autoIndex;
}

bool LocationConfig::isMethodAllowed(const std::string& method) const {
    if (allowedMethods.empty())
        return true;

    for (int i = 0; i < allowedMethods.size(); i++) {
        if (method == allowedMethods[i])
            return true;
    }
    return false;
}

bool LocationConfig::isCGI(const std::string& path) const {
    if (cgiExtension.empty())
        return false;
    
    size_t dot = path.find_last_of('.');
    if (dot == std::string::npos)
        return false;

    std::string extension = path.substr(dot);

    for (size_t i = 0; i < cgiExtension.size(); i++) {
        if (extension == cgiExtension[i])
            return true;
    }
    return false;
}

std::string LocationConfig::getTrueRoot(const ServerConfig& server) const {
    if (!root.empty())
        return root;
    return server.getRoot();
}

bool LocationConfig::isValid() const {
    if (path.empty() || path[0] != '/')
        return false;

    if (uploadEnable && uploadStore.empty())
        return false;
    
    if (!cgiExtension.empty() && cgiPath.empty())
        return false;
    
    if (!redirect.empty() && (redirectCode != 301 && redirectCode != 302))
        return false;
    
    return true;
}
