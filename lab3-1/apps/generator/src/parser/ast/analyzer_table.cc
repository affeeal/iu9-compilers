#include "analyzer_table.h"

#include "ast.h"

namespace parser {

namespace ast {

AnalyzerTable::AnalyzerTable()
    : axiom_({"Program", Symbol::Type::kNonterminal}),
      table_({{{{"Expr", Symbol::Type::kNonterminal},
                {"KW_EPSILON", Symbol::Type::kTerminal}},
               {{"Term", Symbol::Type::kNonterminal},
                {"Expr1", Symbol::Type::kNonterminal}}},
              {{{"Rule", Symbol::Type::kNonterminal},
                {"KW_AXIOM", Symbol::Type::kTerminal}},
               {{"RuleLHS", Symbol::Type::kNonterminal},
                {"ARROW", Symbol::Type::kTerminal},
                {"RuleRHS", Symbol::Type::kNonterminal}}},
              {{{"Rules", Symbol::Type::kNonterminal},
                {"KW_AXIOM", Symbol::Type::kTerminal}},
               {{"Rule", Symbol::Type::kNonterminal},
                {"Rules", Symbol::Type::kNonterminal}}},
              {{{"Term", Symbol::Type::kNonterminal},
                {"TERMINAL", Symbol::Type::kTerminal}},
               {{"Symbol", Symbol::Type::kNonterminal},
                {"Term1", Symbol::Type::kNonterminal}}},
              {{{"Expr", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Term", Symbol::Type::kNonterminal},
                {"Expr1", Symbol::Type::kNonterminal}}},
              {{{"Expr1", Symbol::Type::kNonterminal},
                {"KW_OR", Symbol::Type::kTerminal}},
               {{"KW_OR", Symbol::Type::kTerminal},
                {"Term", Symbol::Type::kNonterminal},
                {"Expr1", Symbol::Type::kNonterminal}}},
              {{{"Rules", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Rule", Symbol::Type::kNonterminal},
                {"Rules", Symbol::Type::kNonterminal}}},
              {{{"Rule", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"RuleLHS", Symbol::Type::kNonterminal},
                {"ARROW", Symbol::Type::kTerminal},
                {"RuleRHS", Symbol::Type::kNonterminal}}},
              {{{"Term1", Symbol::Type::kNonterminal},
                {"KW_OR", Symbol::Type::kTerminal}},
               {}},
              {{{"Term1", Symbol::Type::kNonterminal},
                {"TERMINAL", Symbol::Type::kTerminal}},
               {{"Symbol", Symbol::Type::kNonterminal},
                {"Term1", Symbol::Type::kNonterminal}}},
              {{{"Expr", Symbol::Type::kNonterminal},
                {"TERMINAL", Symbol::Type::kTerminal}},
               {{"Term", Symbol::Type::kNonterminal},
                {"Expr1", Symbol::Type::kNonterminal}}},
              {{{"Term1", Symbol::Type::kNonterminal},
                {"KW_END", Symbol::Type::kTerminal}},
               {}},
              {{{"RuleLHS", Symbol::Type::kNonterminal},
                {"KW_AXIOM", Symbol::Type::kTerminal}},
               {{"KW_AXIOM", Symbol::Type::kTerminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}}},
              {{{"Expr1", Symbol::Type::kNonterminal},
                {"KW_END", Symbol::Type::kTerminal}},
               {}},
              {{{"Program", Symbol::Type::kNonterminal},
                {"KW_AXIOM", Symbol::Type::kTerminal}},
               {{"Rules", Symbol::Type::kNonterminal}}},
              {{{"Symbol", Symbol::Type::kNonterminal},
                {"TERMINAL", Symbol::Type::kTerminal}},
               {{"TERMINAL", Symbol::Type::kTerminal}}},
              {{{"Program", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Rules", Symbol::Type::kNonterminal}}},
              {{{"RuleRHS", Symbol::Type::kNonterminal},
                {"KW_EPSILON", Symbol::Type::kTerminal}},
               {{"Expr", Symbol::Type::kNonterminal},
                {"KW_END", Symbol::Type::kTerminal}}},
              {{{"RuleRHS", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Expr", Symbol::Type::kNonterminal},
                {"KW_END", Symbol::Type::kTerminal}}},
              {{{"RuleRHS", Symbol::Type::kNonterminal},
                {"TERMINAL", Symbol::Type::kTerminal}},
               {{"Expr", Symbol::Type::kNonterminal},
                {"KW_END", Symbol::Type::kTerminal}}},
              {{{"Term1", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Symbol", Symbol::Type::kNonterminal},
                {"Term1", Symbol::Type::kNonterminal}}},
              {{{"Symbol", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"NONTERMINAL", Symbol::Type::kTerminal}}},
              {{{"Term", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"Symbol", Symbol::Type::kNonterminal},
                {"Term1", Symbol::Type::kNonterminal}}},
              {{{"RuleLHS", Symbol::Type::kNonterminal},
                {"NONTERMINAL", Symbol::Type::kTerminal}},
               {{"NONTERMINAL", Symbol::Type::kTerminal}}},
              {{{"Term", Symbol::Type::kNonterminal},
                {"KW_EPSILON", Symbol::Type::kTerminal}},
               {{"KW_EPSILON", Symbol::Type::kTerminal}}},
              {{{"Rules", Symbol::Type::kNonterminal},
                {"END_OF_PROGRAM", Symbol::Type::kTerminal}},
               {}},
              {{{"Program", Symbol::Type::kNonterminal},
                {"END_OF_PROGRAM", Symbol::Type::kTerminal}},
               {{"Rules", Symbol::Type::kNonterminal}}}}) {}

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
