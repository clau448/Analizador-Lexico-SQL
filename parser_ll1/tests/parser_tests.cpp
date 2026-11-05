#include "../Parser.h"
#include <cassert>
#include <iostream>

int main() {
  sql_ll1::Parser parser;
  assert(parser.parse("SELECT * FROM alumnos;").accepted);
  assert(parser.parse("select nombre, edad from alumnos;").accepted);
  assert(parser.parse("SELECT nombre FROM alumnos WHERE edad >= 18;").accepted);
  assert(parser.parse("SELECT nombre FROM alumnos WHERE edad >= 18 AND ciudad = 'Lima';").accepted);
  assert(!parser.parse("SELECT FROM alumnos;").accepted);
  assert(!parser.parse("SELECT nombre alumnos;").accepted);
  std::cout << "OK: motor LL(1) iterativo integrado con Lexer\n";
  return 0;
}
