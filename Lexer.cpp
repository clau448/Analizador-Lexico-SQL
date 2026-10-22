#include "Lexer.h"
#include <cctype>

char Lexer::peek() {
    if (currentPos >= input.length()) return '\0';
    return input[currentPos];
}

char Lexer::advance() {
    if (currentPos >= input.length()) return '\0';
    char c = input[currentPos];
    currentPos++;
    
    if (c == '\n') {
        currentLine++;
        currentCol = 1;
    } else {
        currentCol++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) {
        advance();
    }
}

Token Lexer::getNextToken() {
    skipWhitespace();
    
    Token token;
    token.pos = {currentLine, currentCol};
    
    if (currentPos >= input.length()) {
        token.type = TokenType::TOKEN_EOF;
        token.lexeme = "EOF";
        return token;
    }

    char c = advance();
    token.lexeme = std::string(1, c);
    

    if (std::isalpha(c)) {
        while (std::isalnum(peek())) {
            token.lexeme += advance();
        }
        if (!keywords.isKeyword(token.lexeme, token.type)) {
            token.type = TokenType::IDENTIFIER;
        }
    } else if (c == ';') {
        token.type = TokenType::SEMICOLON;
    } else {
        token.type = TokenType::TOKEN_ERROR;
    }
    
    return token;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token currentToken = getNextToken();
    
    while (currentToken.type != TokenType::TOKEN_EOF) {
        tokens.push_back(currentToken);
        currentToken = getNextToken();
    }
    
    tokens.push_back(currentToken);
    
    return tokens;
}