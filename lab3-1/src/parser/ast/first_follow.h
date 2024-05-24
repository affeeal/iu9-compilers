#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ast.h"

namespace parser {

namespace ast {

using SymbolVectorIter = std::vector<Symbol>::const_iterator;
using TableSymbolSetIter = std::unordered_set<TableSymbol>::const_iterator;

class FirstFollow final {
 public:
  FirstFollow(const Program& program);

  std::unordered_set<TableSymbol> GetFirstSet(SymbolVectorIter b,
                                              const SymbolVectorIter e) const;
  std::pair<TableSymbolSetIter, TableSymbolSetIter> GetFollowSet(
      const std::string& name) const;

 private:
  void BuildFirstSets(const Program& program);
  void BuildFollowSets(const Program& program);

  void PrintSets(auto&& sets) const;

 private:
  std::unordered_map<std::string, std::unordered_set<TableSymbol>> first_sets_,
      follow_sets_;
};

}  // namespace ast

}  // namespace parser
