#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ast.h"

namespace parser {

namespace ast {

class FirstFollow final {
  std::unordered_map<std::string, std::unordered_set<TableSymbol>> first_sets_;

 public:
  FirstFollow(const Program& program);

  using SymbolIter = std::vector<Symbol>::const_iterator;
  std::unordered_set<TableSymbol> GetFirstSet(SymbolIter b, const SymbolIter e);

 private:
  void BuildFirstSets(const Program& program);

  void PrintFirstSets() const;
};

}  // namespace ast

}  // namespace parser
