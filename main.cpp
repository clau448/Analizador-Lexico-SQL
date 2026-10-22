#include <iostream>
#include <vector>
#include "Lexer.h"

int main() {
    std::string sqlQuery = "SELECT nombre, edad FROM estudiantes WHERE edad >= 18;";
    
    Lexer lexer(sqlQuery);
    
    std::vector<Token> tokens = lexer.tokenize();
    
    std::cout << "Lexema\t|\tTipo de token\t|\tLínea\t|\tColumna\n";
    for (const auto& token : tokens) {
        std::cout << token.lexeme << "\t|\t" 
                  << static_cast<int>(token.type) << "\t|\t" 
                  << token.pos.line << "\t|\t" 
                  << token.pos.column << "\n";
    }
    
    return 0;
}