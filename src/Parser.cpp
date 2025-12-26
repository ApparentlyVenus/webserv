/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 19:05:40 by odana             #+#    #+#             */
/*   Updated: 2025/12/26 19:05:41 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        throw std::runtime_error("unexpected token"); // will become a proper exception
    return consume();
}

ServerBlock Parser::parseServer() {
    Token server = expect(WORD);

    if (server.val != "server")
        throw std::runtime_error("expected server token"); // unexpect token exception as well

    Token open = expect(OPEN_BRACE);

    std::vector<Directive> directives;
    std::vector<LocationBlock> locations;
    while (!isAtEnd() && peek().type != CLOSE_BRACE) {
        if (peek().type == WORD && peek().val == "location")
            locations.push_back(parseLocation());
        else
            directives.push_back(parseDirective());
    }

    Token close = expect(CLOSE_BRACE);
    return ServerBlock(server, open, directives, locations, close);
}

LocationBlock Parser::parseLocation() {
    Token location = expect(WORD);

    if (location.val != "location")
        throw std::runtime_error("expected location token"); // unexpected token exception
    
    Token path = expect(WORD);
    Token open = expect(OPEN_BRACE);

    std::vector<Directive> directives;
    while (!isAtEnd() && peek().type != CLOSE_BRACE)
        directives.push_back(parseDirective());

    Token close = expect(CLOSE_BRACE);
    return LocationBlock(location, path, open, directives, close);
}

Directive Parser::parseDirective() {
    Token key = expect(WORD);

    std::vector<Token> values;
    while(!isAtEnd() && peek().type != SEMICOLON)
        values.push_back(expect(WORD));

    Token semiColon = expect(SEMICOLON);
    return Directive(key, values, semiColon);
}