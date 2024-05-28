#pragma once

#include <optional>

// clang-format off
#include <boost/unordered_map.hpp>
// clang-format on

#include "ast.h"

namespace parser {

namespace ast {

class AnalyzerTable final {
  Symbol axiom_;
  boost::unordered_map<std::pair<Symbol, Symbol>, std::vector<Symbol>> table_;

 public:
  AnalyzerTable();

  const Symbol& get_axiom() const noexcept { return axiom_; }
  std::optional<std::pair<SymbolVecIter, SymbolVecIter>> Find(
      const Symbol& nonterminal, const Symbol& symbol) const;
};

}  // namespace ast

}  // namespace parser
