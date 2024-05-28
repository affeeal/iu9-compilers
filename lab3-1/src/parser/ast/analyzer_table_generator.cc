#include "analyzer_table_generator.h"

#include <iostream>

#include "ast.h"
#include "first_follow.h"

namespace parser {

namespace ast {

AnalyzerTableGenerator::AnalyzerTableGenerator(const FirstFollow& first_follow)
    : program_(first_follow.get_program()) {
  for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e;
       ++b) {
    const auto& rule = **b;

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;

      auto first_set =
          first_follow.GetFirstSet(term.SymbolsCbegin(), term.SymbolsCend());
      const auto erased = first_set.erase(kEpsilon);

      for (auto&& symbol : first_set) {
        const auto [_, is_inserted] =
            table_.insert({{rule.get_lhs(), symbol},
                           {term.SymbolsCbegin(), term.SymbolsCend()}});
        // TODO: remove duplication (?), recheck
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }

      if (!erased) {
        continue;
      }

      for (auto [b, e] = first_follow.GetFollowSet(rule.get_lhs()); b != e;
           ++b) {
        const auto [_, is_inserted] = table_.insert(
            {{rule.get_lhs(), *b}, {term.SymbolsCbegin(), term.SymbolsCend()}});
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }
    }
  }
}

void AnalyzerTableGenerator::PrintTable() const {
  for (auto&& [key, value] : table_) {
    const auto [nonterminal, symbol] = key;
    std::cout << '[' << nonterminal.get_name() << ' ' << symbol.get_name()
              << "]: ";
    for (auto [b, e] = value; b != e; ++b) {
      std::cout << b->get_name() << ' ';
    }
    std::cout << '\n';
  }
}

}  // namespace ast

}  // namespace parser
