#ifndef PARSER_HPP
# define PARSER_HPP

# include "LocationConfig.hpp"
# include "ServerConfig.hpp"
# include "Tokenizer.hpp"
# include <stdexcept>

struct Directive {
    Token key;
    std::vector<Token> values;
    Token semiColon;

    Directive(Token key, std::vector<Token> values, Token semiColon);
};

struct LocationBlock {
    Token location;
    Token path;
    Token openBrace;
    std::vector<Directive> directives;
    Token closeBrace;

    LocationBlock(Token location, Token path, Token open, std::vector<Directive> directives, Token close);

};

struct ServerBlock {
    Token server;
    Token openBrace;
    std::vector<Directive> directives;
    std::vector<LocationBlock> locations;
    Token closeBrace;

    ServerBlock(Token server, Token open, std::vector<Directive> directives,
                    std::vector<LocationBlock> locations, Token close);
};

class Parser {
    private:
    std::vector<Token> tokens;
    size_t pos;


    public:
    Parser();
    Parser(const std::vector<Token>& tokens);
    Parser(const Parser& other);
    Parser& operator=(const Parser& other);


    ServerBlock parseServer();
    LocationBlock parseLocation();
    Directive parseDirective();

    private:
    Token expect(TokenType type);
    Token peek() const;
    Token consume();
    bool  isAtEnd() const;
};

#endif


// config_file      := server_block+

// server_block     := WORD("server") OPEN_BRACE directive* location_block* CLOSE_BRACE

// location_block   := WORD("location") WORD(path) OPEN_BRACE directive* CLOSE_BRACE

// directive        := WORD(key) WORD(value)+ SEMICOLON