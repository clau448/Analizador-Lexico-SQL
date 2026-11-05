#include "Grammar.h"
#include <sstream>
#include <stdexcept>

namespace sql_ll1 {

const Symbol Grammar::EPSILON = "epsilon";
const Symbol Grammar::END = "$";

std::string Production::text() const {
  std::ostringstream out;
  out << lhs << " ->";
  for (std::size_t i = 0; i < rhs.size(); ++i) out << " " << rhs[i];
  return out.str();
}

Grammar::Grammar() : start_("Query") {
  add("Query", std::vector<Symbol>{"SELECT", "Projection", "FROM", "IDENTIFIER", "WhereOpt", ";"});
  add("Projection", std::vector<Symbol>{"*"});
  add("Projection", std::vector<Symbol>{"IDENTIFIER", "ProjectionTail"});
  add("ProjectionTail", std::vector<Symbol>{",", "IDENTIFIER", "ProjectionTail"});
  add("ProjectionTail", std::vector<Symbol>{EPSILON});
  add("WhereOpt", std::vector<Symbol>{"WHERE", "Condition"});
  add("WhereOpt", std::vector<Symbol>{EPSILON});
  add("Condition", std::vector<Symbol>{"Comparison", "ConditionTail"});
  add("ConditionTail", std::vector<Symbol>{"LogicOp", "Comparison", "ConditionTail"});
  add("ConditionTail", std::vector<Symbol>{EPSILON});
  add("LogicOp", std::vector<Symbol>{"AND"});
  add("LogicOp", std::vector<Symbol>{"OR"});
  add("Comparison", std::vector<Symbol>{"IDENTIFIER", "RelOp", "Value"});
  add("RelOp", std::vector<Symbol>{"="});
  add("RelOp", std::vector<Symbol>{"<>"});
  add("RelOp", std::vector<Symbol>{"<"});
  add("RelOp", std::vector<Symbol>{">"});
  add("RelOp", std::vector<Symbol>{"<="});
  add("RelOp", std::vector<Symbol>{">="});
  add("Value", std::vector<Symbol>{"IDENTIFIER"});
  add("Value", std::vector<Symbol>{"INTEGER"});
  add("Value", std::vector<Symbol>{"DECIMAL"});
  add("Value", std::vector<Symbol>{"STRING"});
  collectSymbols();
  computeFirst();
  computeFollow();
  buildTable();
}

void Grammar::add(const Symbol& lhs, const std::vector<Symbol>& rhs) {
  Production p = {static_cast<int>(productions_.size()) + 1, lhs, rhs};
  productions_.push_back(p);
  nonTerminals_.insert(lhs);
}

void Grammar::collectSymbols() {
  for (std::size_t i = 0; i < productions_.size(); ++i) {
    for (std::size_t j = 0; j < productions_[i].rhs.size(); ++j) {
      const Symbol& s = productions_[i].rhs[j];
      if (s != EPSILON && nonTerminals_.count(s) == 0) terminals_.insert(s);
    }
  }
  terminals_.insert(END);
}

std::set<Symbol> Grammar::firstOfSequence(const std::vector<Symbol>& sequence) const {
  std::set<Symbol> result;
  bool nullable = true;
  for (std::size_t i = 0; i < sequence.size() && nullable; ++i) {
    const Symbol& s = sequence[i];
    if (s == EPSILON) { result.insert(EPSILON); break; }
    std::map<Symbol, std::set<Symbol> >::const_iterator it = first_.find(s);
    if (it == first_.end()) { result.insert(s); nullable = false; break; }
    nullable = it->second.count(EPSILON) != 0;
    for (std::set<Symbol>::const_iterator x = it->second.begin(); x != it->second.end(); ++x)
      if (*x != EPSILON) result.insert(*x);
  }
  if (sequence.empty() || nullable) result.insert(EPSILON);
  return result;
}

void Grammar::computeFirst() {
  for (std::set<Symbol>::const_iterator it = terminals_.begin(); it != terminals_.end(); ++it)
    first_[*it].insert(*it);
  first_[EPSILON].insert(EPSILON);
  for (std::set<Symbol>::const_iterator it = nonTerminals_.begin(); it != nonTerminals_.end(); ++it)
    first_[*it];
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t i = 0; i < productions_.size(); ++i) {
      std::set<Symbol> seqFirst = firstOfSequence(productions_[i].rhs);
      std::size_t old = first_[productions_[i].lhs].size();
      first_[productions_[i].lhs].insert(seqFirst.begin(), seqFirst.end());
      changed = changed || old != first_[productions_[i].lhs].size();
    }
  }
}

void Grammar::computeFollow() {
  for (std::set<Symbol>::const_iterator it = nonTerminals_.begin(); it != nonTerminals_.end(); ++it)
    follow_[*it];
  follow_[start_].insert(END);
  bool changed = true;
  while (changed) {
    changed = false;
    for (std::size_t i = 0; i < productions_.size(); ++i) {
      const Production& p = productions_[i];
      for (std::size_t j = 0; j < p.rhs.size(); ++j) {
        const Symbol& b = p.rhs[j];
        if (nonTerminals_.count(b) == 0) continue;
        std::vector<Symbol> beta(p.rhs.begin() + j + 1, p.rhs.end());
        std::set<Symbol> firstBeta = firstOfSequence(beta);
        std::size_t old = follow_[b].size();
        for (std::set<Symbol>::const_iterator x = firstBeta.begin(); x != firstBeta.end(); ++x)
          if (*x != EPSILON) follow_[b].insert(*x);
        if (beta.empty() || firstBeta.count(EPSILON))
          follow_[b].insert(follow_[p.lhs].begin(), follow_[p.lhs].end());
        changed = changed || old != follow_[b].size();
      }
    }
  }
}

void Grammar::buildTable() {
  for (std::size_t i = 0; i < productions_.size(); ++i) {
    const Production& p = productions_[i];
    std::set<Symbol> selection = firstOfSequence(p.rhs);
    if (selection.erase(EPSILON)) selection.insert(follow_[p.lhs].begin(), follow_[p.lhs].end());
    for (std::set<Symbol>::const_iterator t = selection.begin(); t != selection.end(); ++t) {
      std::pair<Symbol, Symbol> cell(p.lhs, *t);
      std::map<std::pair<Symbol, Symbol>, int>::const_iterator existing = table_.find(cell);
      if (existing != table_.end() && existing->second != p.id) {
        std::ostringstream msg;
        msg << "Conflicto M[" << p.lhs << ", " << *t << "]: reglas "
            << existing->second << " y " << p.id;
        conflicts_.push_back(msg.str());
      } else table_[cell] = p.id;
    }
  }
}

const Production* Grammar::lookup(const Symbol& nonTerminal, const Symbol& lookahead) const {
  std::map<std::pair<Symbol, Symbol>, int>::const_iterator it = table_.find(std::make_pair(nonTerminal, lookahead));
  if (it == table_.end()) return 0;
  return &productions_.at(static_cast<std::size_t>(it->second - 1));
}

std::set<Symbol> Grammar::expected(const Symbol& nonTerminal) const {
  std::set<Symbol> result;
  for (std::map<std::pair<Symbol, Symbol>, int>::const_iterator it = table_.begin(); it != table_.end(); ++it)
    if (it->first.first == nonTerminal) result.insert(it->first.second);
  return result;
}

bool Grammar::isNonTerminal(const Symbol& symbol) const { return nonTerminals_.count(symbol) != 0; }
const Symbol& Grammar::start() const { return start_; }
const std::vector<Production>& Grammar::productions() const { return productions_; }
const std::set<Symbol>& Grammar::nonTerminals() const { return nonTerminals_; }
const std::set<Symbol>& Grammar::terminals() const { return terminals_; }
const std::map<Symbol, std::set<Symbol> >& Grammar::firstSets() const { return first_; }
const std::map<Symbol, std::set<Symbol> >& Grammar::followSets() const { return follow_; }
const std::map<std::pair<Symbol, Symbol>, int>& Grammar::parseTable() const { return table_; }
std::vector<std::string> Grammar::conflicts() const { return conflicts_; }

Symbol Grammar::terminalFor(TokenType type) {
  switch (type) {
    case TokenType::KEYWORD_SELECT: return "SELECT";
    case TokenType::KEYWORD_FROM: return "FROM";
    case TokenType::KEYWORD_WHERE: return "WHERE";
    case TokenType::KEYWORD_AND: return "AND";
    case TokenType::KEYWORD_OR: return "OR";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::INTEGER_LITERAL: return "INTEGER";
    case TokenType::DECIMAL_LITERAL: return "DECIMAL";
    case TokenType::STRING_LITERAL: return "STRING";
    case TokenType::OP_EQUAL: return "=";
    case TokenType::OP_NOT_EQUAL: return "<>";
    case TokenType::OP_LESS: return "<";
    case TokenType::OP_GREATER: return ">";
    case TokenType::OP_LESS_EQUAL: return "<=";
    case TokenType::OP_GREATER_EQUAL: return ">=";
    case TokenType::COMMA: return ",";
    case TokenType::SEMICOLON: return ";";
    case TokenType::STAR: return "*";
    case TokenType::TOKEN_EOF: return END;
    case TokenType::TOKEN_ERROR: return "LEXICAL_ERROR";
  }
  return "UNKNOWN";
}

std::string Grammar::tokenName(TokenType type) { return terminalFor(type); }

} // namespace sql_ll1
