#pragma once

// clang-format off
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
// clang-format on

#include "ast.h"

namespace semantics {

class FirstFollow final {
 public:
  FirstFollow(std::shared_ptr<const Program> program);

  std::shared_ptr<const Program> get_program() const noexcept {
    return program_;
  }

  boost::unordered_set<parser::Symbol> GetFirstSet(
      parser::SymbolVecIter b, const parser::SymbolVecIter e) const;
  std::pair<parser::SymbolSetIter, parser::SymbolSetIter> GetFollowSet(
      const parser::Symbol& nonterminal) const;

 private:
  void BuildFirstSets();
  void BuildFollowSets();

  void PrintSets(auto&& sets) const;

 private:
  std::shared_ptr<const Program> program_;
  boost::unordered_map<parser::Symbol, boost::unordered_set<parser::Symbol>>
      first_sets_, follow_sets_;
};

}  // namespace semantics
