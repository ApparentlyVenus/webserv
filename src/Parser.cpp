#include "../inc/Parser.hpp"

Parser::Parser() : pos(0) {}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

Parser::Parser(const Parser& other) : tokens(other.tokens), pos(other.pos) {}

Parser& Parser::operator=(const Parser& other) {
    if (this != &other) {
        tokens = other.tokens;
        pos = other.pos;
    }
    return *this;
}

Parser::~Parser() {}

Directive::Directive(Token key, std::vector<Token> tokens, Token semiColon) 
    :   key(key), values(tokens), semiColon(semiColon) {}

LocationBlock::LocationBlock(Token location, Token path, Token open,
    std::vector<Directive> directives, Token close) 
    :   location(location), path(path), openBrace(open),
        directives(directives), closeBrace(close) {}

ServerBlock::ServerBlock(Token server, Token open, std::vector<Directive> directives,
    std::vector<LocationBlock> locations, Token close) 
    :   server(server), openBrace(open), directives(directives), 
        locations(locations), closeBrace(close) {}

Token Parser::peek() const {
    if (pos >= tokens.size())
        return tokens.at(tokens.size() - 1);
    return tokens.at(pos);
}

Token Parser::consume() {
    if (pos >= tokens.size())
        return tokens[tokens.size() - 1];
    return tokens[pos++];
}

bool Parser::isAtEnd() const {
    return peek().type == END_OF_FILE;
}

Token Parser::expect(TokenType type) {
    Token cur = peek();
    if (cur.type != type)
        throw std::runtime_error("unexpected token");
    return consume();
}

ServerBlock Parser::parseServer() {
    Token server = expect(WORD);

    if (server.val == "server") {
        Token open = expect(OPEN_BRACE);
        Token cur = open;

        std::vector<Directive> directives;
        while (cur.val != "location")
            directives.push_back(parseDirective());
        
        std::vector<LocationBlock> locations;
        while (cur.val == "location")
            locations.push_back(parseLocation());
        
        Token close = expect(CLOSE_BRACE);
    }
    return ServerBlock(server, open, directives, locations, close);

}

LocationBlock Parser::parseLocation() {
    Token location = consume();
    if (location.val == "locaion") {
        Token path = expect(WORD);
        Token open = expect(OPEN_BRACE);
        Token cur = open;

        std::vector<Directive> directives;
        while (cur.type == WORD)
            directives.push_back(parseDirective())
        
        Token close = expect(CLOSE_BRACE);
    }


    return LocationBlock();
}