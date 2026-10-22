#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "Token.h"
#include <unordered_map>
#include <string>

class Keywords {
private:
    std::unordered_map<std::string, TokenType> reservedWords;
public:
    Keywords() {
        reservedWords["SELECT"] = TokenType::KEYWORD_SELECT;
        reservedWords["FROM"] = TokenType::KEYWORD_FROM;
        reservedWords["WHERE"] = TokenType::KEYWORD_WHERE;
        reservedWords["AND"] = TokenType::KEYWORD_AND;
        reservedWords["OR"] = TokenType::KEYWORD_OR;
    }
    
    bool isKeyword(const std::string& lexeme, TokenType& outType) {
        auto it = reservedWords.find(lexeme);
        if (it != reservedWords.end()) {
            outType = it->second;
            return true;
        }
        return false;
    }
};

#endif
