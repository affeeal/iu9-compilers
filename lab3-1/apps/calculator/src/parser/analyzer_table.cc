#include "analyzer_table.h"

#include "symbol.h"

namespace parser {

AnalyzerTable::AnalyzerTable()
    : axiom_({"E", Symbol::Type::kNonterminal}),
      table_(
          {{{{"T1", Symbol::Type::kNonterminal},
             {"END_OF_PROGRAM", Symbol::Type::kTerminal}},
            {}},
           {{{"F", Symbol::Type::kNonterminal}, {"(", Symbol::Type::kTerminal}},
            {{"(", Symbol::Type::kTerminal},
             {"E", Symbol::Type::kNonterminal},
             {")", Symbol::Type::kTerminal}}},
           {{{"E1", Symbol::Type::kNonterminal},
             {")", Symbol::Type::kTerminal}},
            {}},
           {{{"E1", Symbol::Type::kNonterminal},
             {"END_OF_PROGRAM", Symbol::Type::kTerminal}},
            {}},
           {{{"E", Symbol::Type::kNonterminal}, {"(", Symbol::Type::kTerminal}},
            {{"T", Symbol::Type::kNonterminal},
             {"E1", Symbol::Type::kNonterminal}}},
           {{{"E", Symbol::Type::kNonterminal}, {"n", Symbol::Type::kTerminal}},
            {{"T", Symbol::Type::kNonterminal},
             {"E1", Symbol::Type::kNonterminal}}},
           {{{"T1", Symbol::Type::kNonterminal},
             {")", Symbol::Type::kTerminal}},
            {}},
           {{{"T1", Symbol::Type::kNonterminal},
             {"+", Symbol::Type::kTerminal}},
            {}},
           {{{"T1", Symbol::Type::kNonterminal},
             {"*", Symbol::Type::kTerminal}},
            {{"*", Symbol::Type::kTerminal},
             {"F", Symbol::Type::kNonterminal},
             {"T1", Symbol::Type::kNonterminal}}},
           {{{"T", Symbol::Type::kNonterminal}, {"n", Symbol::Type::kTerminal}},
            {{"F", Symbol::Type::kNonterminal},
             {"T1", Symbol::Type::kNonterminal}}},
           {{{"F", Symbol::Type::kNonterminal}, {"n", Symbol::Type::kTerminal}},
            {{"n", Symbol::Type::kTerminal}}},
           {{{"E1", Symbol::Type::kNonterminal},
             {"+", Symbol::Type::kTerminal}},
            {{"+", Symbol::Type::kTerminal},
             {"T", Symbol::Type::kNonterminal},
             {"E1", Symbol::Type::kNonterminal}}},
           {{{"T", Symbol::Type::kNonterminal}, {"(", Symbol::Type::kTerminal}},
            {{"F", Symbol::Type::kNonterminal},
             {"T1", Symbol::Type::kNonterminal}}}}) {}

std::optional<std::pair<SymbolVecIter, SymbolVecIter>> AnalyzerTable::Find(
    const Symbol& nonterminal, const Symbol& terminal) const {
  if (const auto it = table_.find({nonterminal, terminal});
      it != table_.cend()) {
    const auto& symbols = it->second;
    return std::make_pair(symbols.cbegin(), symbols.cend());
  }
  return std::nullopt;
}

}  // namespace parser
