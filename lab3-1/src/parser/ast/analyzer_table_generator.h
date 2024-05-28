#pragma once

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include <unordered_map>

#include "ast.h"
#include "first_follow.h"
#include "symbol_table.h"

namespace parser {

namespace ast {

class AnalyzerTableGenerator final {
 public:
  using Key = std::pair<const Nonterminal*, const ISymbol*>;
  using Value = std::pair<ISymbolVecIter, ISymbolVecIter>;

  AnalyzerTableGenerator(const FirstFollow& first_follow);

  void PrintTable() const;

 private:
  std::unordered_map<Key, Value, boost::hash<Key>> table_;
  std::shared_ptr<const Program> program_;
};

}  // namespace ast

}  // namespace parser
