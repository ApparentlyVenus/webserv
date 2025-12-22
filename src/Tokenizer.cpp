/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:48:44 by odana             #+#    #+#             */
/*   Updated: 2025/12/22 11:12:16 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"

Token::Token(TokenType type) : type(type), val("") {}

Token::Token(TokenType type, const std::string& val) : type(type), val(val) {}

Tokenizer::Tokenizer(const std::string& fileContent) : content(fileContent), pos(0) {}

std::vector<Token> Tokenizer::tokenize() {
    while (pos < content.size()) {
        
        skipWhitespace();
        
        if (pos >= content.size())
            break;
        
        char c = content[pos];
        
        if (c == '#') {
            skipComment();
            continue;
        }

        if (c == '{') {
            tokens.push_back(Token(OPEN_BRACE));
            pos++;
        }

        else if (c == '}') {
            tokens.push_back(Token(CLOSE_BRACE));
            pos++;
        }
        
        else if (c == ';') {
            tokens.push_back(Token(SEMICOLON));
            pos++;
        }
        
        else {
            std::string word = readWord();
            if (!word.empty())
                tokens.push_back(Token(WORD, word));
        }
    }
    
    tokens.push_back(Token(END_OF_FILE));
    return tokens;
}

std::string Tokenizer::readWord() {
    std::string word;
    while (pos < content.size()) {
        char c = content[pos];

        if (isWhiteSpace(c) || isSpecialChar(c))
            break;
        word += c;
        pos++;
    }
    return word;
}

void Tokenizer::skipWhitespace() {
    while (pos < content.size() && isWhiteSpace(content[pos]))
        pos++;
}

void Tokenizer::skipComment() {
    pos++;

    while (pos < content.size() && content[pos] != '\n')
        pos++;
}

bool Tokenizer::isWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Tokenizer::isSpecialChar(char c) {
    return c == '{' || c == '}' || c == ';' || c == '#';
}