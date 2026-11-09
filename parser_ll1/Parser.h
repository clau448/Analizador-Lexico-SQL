#ifndef PARSER_LL1_PARSER_H
#define PARSER_LL1_PARSER_H

#include "Grammar.h"
#include <string>
#include <vector>

namespace sql_ll1 {

struct ParseError {
  int line;
  int column;
  std::string found;
  std::vector<std::string> expected;
  std::string message;
};

struct TraceStep {
  std::size_t step;
  std::string stack;
  std::string input;
  std::string action;
};

struct ParseResult {
  bool accepted;
  std::vector<ParseError> errors;
  std::vector<TraceStep> trace;
};

class Parser {
public:
  explicit Parser(bool traceEnabled = false);
  ParseResult parse(const std::string& sql) const;
  ParseResult parseTokens(const std::vector<Token>& tokens) const;
  const Grammar& grammar() const;

private:
  Grammar grammar_;
  bool traceEnabled_;
};

} // namespace sql_ll1

#endif
