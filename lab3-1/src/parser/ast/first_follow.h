#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ast.h"

namespace parser {

namespace ast {

class FirstFollow final {
 public:
  FirstFollow(std::shared_ptr<const Program> program);

  std::shared_ptr<const Program> get_program() const noexcept { return program_; }

  std::unordered_set<const ISymbol*> GetFirstSet(ISymbolVecIter b,
                                                 const ISymbolVecIter e) const;
  std::pair<ISymbolSetIter, ISymbolSetIter> GetFollowSet(
      const Nonterminal* const nonterminal) const;

 private:
  void BuildFirstSets();
  void BuildFollowSets();

  void PrintSets(auto&& sets) const;

 private:
  std::shared_ptr<const Program> program_;
  std::unordered_map<const Nonterminal*, std::unordered_set<const ISymbol*>>
      first_sets_, follow_sets_;
};

}  // namespace ast

}  // namespace parser
