#include "../Grammar.h"
#include "../../Lexer.h"
#include <cassert>
#include <iostream>

using sql_ll1::Grammar;

int main() {
  Grammar grammar;
  assert(grammar.conflicts().empty());
  assert(grammar.productions().size() == 23);
  assert(grammar.firstSets().at("Query").count("SELECT") == 1);
  assert(grammar.firstSets().at("Projection").count("*") == 1);
  assert(grammar.firstSets().at("Projection").count("IDENTIFIER") == 1);
  assert(grammar.followSets().at("ProjectionTail").count("FROM") == 1);
  assert(grammar.followSets().at("WhereOpt").count(";") == 1);
  assert(grammar.followSets().at("ConditionTail").count(";") == 1);
  assert(grammar.lookup("Projection", "*") != 0);
  assert(grammar.lookup("WhereOpt", ";") != 0);

  Lexer lexer("SELECT * FROM alumnos;");
  std::vector<Token> tokens = lexer.tokenize();
  assert(tokens.size() == 6);
  assert(tokens[1].type == TokenType::STAR);
  std::cout << "OK: gramatica LL(1), FIRST/FOLLOW y tabla sin conflictos\n";
  return 0;
}
