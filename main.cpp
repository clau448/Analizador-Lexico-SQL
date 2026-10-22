#include <iostream>
#include <vector>
#include "Lexer.h"

int main() {
    // 1. Módulo de lectura de entrada[cite: 3].
    std::string sqlQuery = "SELECT nombre, edad FROM estudiantes WHERE edad >= 18;";
    
    Lexer lexer(sqlQuery);
    
    // 2. Transformar cadena en secuencia de tokens[cite: 1].
    std::vector<Token> tokens = lexer.tokenize();
    
    // 3. Interfaz de salida[cite: 3].
    std::cout << "Lexema\t|\tTipo de token\t|\tLínea\t|\tColumna\n";
    for (const auto& token : tokens) {
        std::cout << token.lexeme << "\t|\t" 
                  << static_cast<int>(token.type) << "\t|\t" 
                  << token.pos.line << "\t|\t" 
                  << token.pos.column << "\n";
    }
    
    return 0;
}