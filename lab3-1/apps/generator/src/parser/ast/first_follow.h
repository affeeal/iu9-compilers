#pragma once

// clang-format off
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
// clang-format on

#include "ast.h"

namespace parser {

namespace ast {

class FirstFollow final {
 public:
  FirstFollow(std::shared_ptr<const Program> program);

  std::shared_ptr<const Program> get_program() const noexcept {
    return program_;
  }

  boost::unordered_set<Symbol> GetFirstSet(SymbolVecIter b,
                                           const SymbolVecIter e) const;
  std::pair<SymbolSetIter, SymbolSetIter> GetFollowSet(
      const Symbol& nonterminal) const;

 private:
  void BuildFirstSets();
  void BuildFollowSets();

  void PrintSets(auto&& sets) const;

 private:
  std::shared_ptr<const Program> program_;
  boost::unordered_map<Symbol, boost::unordered_set<Symbol>> first_sets_,
      follow_sets_;
};

}  // namespace ast

}  // namespace parser
