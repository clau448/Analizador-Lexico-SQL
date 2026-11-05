#include "Parser.h"
#include "../Lexer.h"
#include <algorithm>
#include <sstream>

namespace sql_ll1 {
namespace {

std::string join(const std::vector<std::string>& items, const std::string& separator) {
  std::ostringstream out;
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i) out << separator;
    out << items[i];
  }
  return out.str();
}

std::string stackText(const std::vector<Symbol>& stack) {
  std::vector<std::string> display;
  for (std::vector<Symbol>::const_reverse_iterator it = stack.rbegin(); it != stack.rend(); ++it)
    display.push_back(*it);
  return join(display, " ");
}

std::string remainingInput(const std::vector<Token>& tokens, std::size_t index) {
  std::vector<std::string> display;
  for (std::size_t i = index; i < tokens.size(); ++i) display.push_back(tokens[i].lexeme);
  return join(display, " ");
}

std::vector<std::string> asVector(const std::set<Symbol>& values) {
  return std::vector<std::string>(values.begin(), values.end());
}

} // namespace

Parser::Parser(bool traceEnabled) : traceEnabled_(traceEnabled) {}

ParseResult Parser::parse(const std::string& sql) const {
  Lexer lexer(sql);
  return parseTokens(lexer.tokenize());
}

ParseResult Parser::parseTokens(const std::vector<Token>& supplied) const {
  ParseResult result;
  result.accepted = false;
  std::vector<Token> tokens = supplied;
  if (tokens.empty() || tokens.back().type != TokenType::TOKEN_EOF) {
    Token eof = {TokenType::TOKEN_EOF, "EOF", {1, 1}};
    if (!tokens.empty()) eof.pos = tokens.back().pos;
    tokens.push_back(eof);
  }

  std::vector<Symbol> stack;
  stack.push_back(Grammar::END);
  stack.push_back(grammar_.start());
  std::size_t index = 0;
  std::size_t step = 1;

  while (!stack.empty()) {
    const Token& lookaheadToken = tokens[index];
    Symbol lookahead = Grammar::terminalFor(lookaheadToken.type);
    Symbol top = stack.back();
    std::string action;

    if (lookaheadToken.type == TokenType::TOKEN_ERROR) {
      ParseError error = {lookaheadToken.pos.line, lookaheadToken.pos.column,
                          lookaheadToken.lexeme, std::vector<std::string>(),
                          "Error lexico: token no reconocido"};
      result.errors.push_back(error);
      action = "rechazar: " + error.message;
    } else if (grammar_.isNonTerminal(top)) {
      const Production* production = grammar_.lookup(top, lookahead);
      if (!production) {
        ParseError error = {lookaheadToken.pos.line, lookaheadToken.pos.column,
                            lookaheadToken.lexeme, asVector(grammar_.expected(top)),
                            "No existe entrada en la tabla para M[" + top + ", " + lookahead + "]"};
        result.errors.push_back(error);
        action = "rechazar: " + error.message;
      } else {
        stack.pop_back();
        for (std::vector<Symbol>::const_reverse_iterator it = production->rhs.rbegin();
             it != production->rhs.rend(); ++it)
          if (*it != Grammar::EPSILON) stack.push_back(*it);
        action = "aplicar " + production->text();
      }
    } else if (top == lookahead) {
      stack.pop_back();
      ++index;
      action = "match " + lookahead;
    } else {
      ParseError error = {lookaheadToken.pos.line, lookaheadToken.pos.column,
                          lookaheadToken.lexeme, std::vector<std::string>(1, top),
                          "Terminal inesperado"};
      result.errors.push_back(error);
      action = "rechazar: se esperaba " + top;
    }

    if (traceEnabled_) result.trace.push_back(TraceStep{step++, stackText(stack), remainingInput(tokens, index), action});
    if (!result.errors.empty()) return result;
  }

  result.accepted = index == tokens.size();
  if (!result.accepted) {
    const Token& token = tokens[index < tokens.size() ? index : tokens.size() - 1];
    result.errors.push_back(ParseError{token.pos.line, token.pos.column, token.lexeme,
                                      std::vector<std::string>(1, Grammar::END),
                                      "Quedaron tokens sin consumir"});
  }
  return result;
}

const Grammar& Parser::grammar() const { return grammar_; }

} // namespace sql_ll1
