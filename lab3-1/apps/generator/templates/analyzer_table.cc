#include "analyzer_table.h"

#include "ast.h"

namespace parser {

namespace ast {

// clang-format off
AnalyzerTable::AnalyzerTable() : axiom_(%1%), table_(%2%) {}
// clang-format on

std::optional<std::pair<SymbolVecIter, SymbolVecIter>> AnalyzerTable::Find(
    const Symbol& nonterminal, const Symbol& symbol) const {
  if (const auto it = table_.find({nonterminal, symbol}); it != table_.cend()) {
    const auto& symbols = it->second;
    return std::make_pair(symbols.cbegin(), symbols.cend());
  }
  return std::nullopt;
}

}  // namespace ast

}  // namespace parser
