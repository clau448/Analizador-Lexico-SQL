#include "../Parser.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

void requireAccepted(sql_ll1::Parser& parser, const std::string& sql) {
  sql_ll1::ParseResult result = parser.parse(sql);
  if (!result.accepted) std::cerr << "Debio aceptar: " << sql << "\n";
  assert(result.accepted && result.errors.empty());
}

void requireRejected(sql_ll1::Parser& parser, const std::string& sql) {
  sql_ll1::ParseResult result = parser.parse(sql);
  if (result.accepted) std::cerr << "Debio rechazar: " << sql << "\n";
  assert(!result.accepted && result.errors.size() == 1);
  assert(result.errors[0].line >= 1 && result.errors[0].column >= 1);
  assert(!result.errors[0].message.empty());
}

} // namespace

int main() {
  sql_ll1::Parser parser;
  const std::vector<std::string> valid = {
    "SELECT * FROM t;",
    "select id from users;",
    "SELECT id, name, score FROM students;",
    "SELECT id FROM t WHERE id = 1;",
    "SELECT id FROM t WHERE id <> 1;",
    "SELECT id FROM t WHERE id < 1;",
    "SELECT id FROM t WHERE id > 1;",
    "SELECT id FROM t WHERE id <= 1;",
    "SELECT id FROM t WHERE id >= 1.5;",
    "SELECT id FROM t WHERE name = 'Ana' OR manager = jefe AND age >= 18;"
  };
  const std::vector<std::string> invalid = {
    "", "SELECT", "SELECT FROM t;", "SELECT id t;", "SELECT id FROM;",
    "SELECT id FROM t", "SELECT id, FROM t;", "SELECT * , id FROM t;",
    "WHERE id = 1 SELECT id FROM t;", "SELECT id FROM t WHERE;",
    "SELECT id FROM t WHERE id 1;", "SELECT id FROM t WHERE id =;",
    "SELECT id FROM t WHERE id = 1 AND;", "SELECT id FROM t #;"
  };
  for (std::size_t i = 0; i < valid.size(); ++i) requireAccepted(parser, valid[i]);
  for (std::size_t i = 0; i < invalid.size(); ++i) requireRejected(parser, invalid[i]);

  sql_ll1::ParseResult positioned = parser.parse("SELECT id FROM t\nWHERE id = 1 AND\n;");
  assert(!positioned.accepted);
  assert(positioned.errors[0].line == 3 && positioned.errors[0].column == 1);
  assert(!positioned.errors[0].expected.empty());

  sql_ll1::Parser traced(true);
  sql_ll1::ParseResult trace = traced.parse("SELECT * FROM t;");
  assert(trace.accepted && trace.trace.size() > 5);
  assert(trace.trace.front().stack == "Query $");
  assert(trace.trace.front().input.find("SELECT") == 0);
  assert(trace.trace.front().action.find("aplicar Query") == 0);
  assert(trace.trace.back().action == "match $");

  std::cout << "OK: " << valid.size() << " validas, " << invalid.size()
            << " invalidas, posiciones y trazador\n";
  return 0;
}
