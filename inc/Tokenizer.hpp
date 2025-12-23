/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:48:38 by odana             #+#    #+#             */
/*   Updated: 2025/12/22 11:06:41 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <string>
# include <vector>

enum TokenType {
    WORD,
    OPEN_BRACE,
    CLOSE_BRACE,
    SEMICOLON,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string val;
    
    Token(TokenType type);
    Token(TokenType type, const std::string& val);
};

class Tokenizer {
    private:
    
    std::string content;
    size_t pos;
    std::vector<Token> tokens;
    
    public:
    
    Tokenizer(const std::string& fileContent);
    std::vector<Token> tokenize();

    private:
    
    void skipWhitespace();
    void skipComment();
    bool isSpecialChar(char c);
    bool isWhiteSpace(char c);
    std::string readWord();
};

#endif