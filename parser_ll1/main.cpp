#include "Parser.h"
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
  bool trace = false;
  std::ostringstream sql;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--trace") trace = true;
    else { if (sql.tellp() > 0) sql << " "; sql << argv[i]; }
  }
  if (sql.str().empty()) sql << std::cin.rdbuf();

  sql_ll1::Parser parser(trace);
  sql_ll1::ParseResult result = parser.parse(sql.str());
  if (trace) {
    std::cout << "PASO | PILA | ENTRADA | ACCION\n";
    for (std::size_t i = 0; i < result.trace.size(); ++i) {
      const sql_ll1::TraceStep& s = result.trace[i];
      std::cout << s.step << " | " << s.stack << " | " << s.input << " | " << s.action << "\n";
    }
  }
  if (result.accepted) {
    std::cout << "ACEPTADA\n";
    return 0;
  }
  for (std::size_t i = 0; i < result.errors.size(); ++i) {
    const sql_ll1::ParseError& e = result.errors[i];
    std::cerr << "ERROR " << e.line << ":" << e.column << ": " << e.message
              << "; encontrado '" << e.found << "'";
    if (!e.expected.empty()) {
      std::cerr << "; esperados:";
      for (std::size_t j = 0; j < e.expected.size(); ++j) std::cerr << " " << e.expected[j];
    }
    std::cerr << "\n";
  }
  return 1;
}
