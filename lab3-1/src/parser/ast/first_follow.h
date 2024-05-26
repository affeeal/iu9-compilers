#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ast.h"

namespace parser {

namespace ast {

using ISymbolVecIter = std::vector<const ISymbol*>::const_iterator;
using ISymbolSetIter = std::unordered_set<const ISymbol*>::const_iterator;

class FirstFollow final {
 public:
  FirstFollow(std::shared_ptr<const Program> program);

  std::unordered_set<const ISymbol*> GetFirstSet(ISymbolVecIter b,
                                                 const ISymbolVecIter e) const;
  std::pair<ISymbolSetIter, ISymbolSetIter> GetFollowSet(
      const Nonterminal* const nonterminal) const;

 private:
  void BuildFirstSets();
  void BuildFollowSets();

 private:
  std::shared_ptr<const Program> program_;
  std::unordered_map<const Nonterminal*, std::unordered_set<const ISymbol*>>
      first_sets_, follow_sets_;
};

}  // namespace ast

}  // namespace parser
