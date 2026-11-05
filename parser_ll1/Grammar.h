#ifndef PARSER_LL1_GRAMMAR_H
#define PARSER_LL1_GRAMMAR_H

#include "../Token.h"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace sql_ll1 {

typedef std::string Symbol;

struct Production {
  int id;
  Symbol lhs;
  std::vector<Symbol> rhs;
  std::string text() const;
};

class Grammar {
public:
  static const Symbol EPSILON;
  static const Symbol END;

  Grammar();

  const Symbol& start() const;
  const std::vector<Production>& productions() const;
  const std::set<Symbol>& nonTerminals() const;
  const std::set<Symbol>& terminals() const;
  const std::map<Symbol, std::set<Symbol> >& firstSets() const;
  const std::map<Symbol, std::set<Symbol> >& followSets() const;
  const std::map<std::pair<Symbol, Symbol>, int>& parseTable() const;

  const Production* lookup(const Symbol& nonTerminal,
                           const Symbol& lookahead) const;
  std::set<Symbol> expected(const Symbol& nonTerminal) const;
  std::vector<std::string> conflicts() const;
  bool isNonTerminal(const Symbol& symbol) const;

  static Symbol terminalFor(TokenType type);
  static std::string tokenName(TokenType type);

private:
  Symbol start_;
  std::vector<Production> productions_;
  std::set<Symbol> nonTerminals_;
  std::set<Symbol> terminals_;
  std::map<Symbol, std::set<Symbol> > first_;
  std::map<Symbol, std::set<Symbol> > follow_;
  std::map<std::pair<Symbol, Symbol>, int> table_;
  std::vector<std::string> conflicts_;

  void add(const Symbol& lhs, const std::vector<Symbol>& rhs);
  void collectSymbols();
  void computeFirst();
  void computeFollow();
  void buildTable();
  std::set<Symbol> firstOfSequence(const std::vector<Symbol>& sequence) const;
};

} // namespace sql_ll1

#endif
