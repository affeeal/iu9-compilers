#include "analyzer_table_generator.h"

#include "ast.h"
#include "first_follow.h"
#include <iostream>

namespace parser {

namespace ast {

AnalyzerTableGenerator::AnalyzerTableGenerator(const FirstFollow& first_follow)
: program_(first_follow.get_program()) {
  const auto* const epsilon = program_->get_symbol_table()->GetEpsilon();

  for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e; ++b) {
    const auto& rule = **b;

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;

      auto first_set =
          first_follow.GetFirstSet(term.SymsCbegin(), term.SymsCend());
      const auto erased = first_set.erase(epsilon);
      
      for (auto&& symbol : first_set) {
        const auto [_, is_inserted] = table_.insert(
            {{rule.get_lhs(), symbol}, {term.SymsCbegin(), term.SymsCend()}});
        // TODO: remove duplication (?), recheck
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }

      if (!erased) {
        continue;
      }

      for (auto [b, e] = first_follow.GetFollowSet(rule.get_lhs()); b != e; ++b) {
        const auto [_, is_inserted] = table_.insert(
            {{rule.get_lhs(), *b}, {term.SymsCbegin(), term.SymsCend()}});
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }
    }
  }
}

void AnalyzerTableGenerator::PrintTable() const {
  const auto symbol_table = program_->get_symbol_table();
  for (auto&& [key, value] : table_) {
    const auto [nonterminal, symbol] = key;
    std::cout << '[' << nonterminal->get_name() << ' '; 
    if (const auto* const terminal = dynamic_cast<const Terminal*>(symbol)) {
      std::cout << terminal->get_name();
    } else if (symbol == symbol_table->GetEpsilon()) {
      std::cout << "ε";
    } else if (symbol == symbol_table->GetDollar()) {
      std::cout << '$';
    }
    std::cout << "]: ";

    for (auto [b, e] = value; b != e; ++b) {
      if (const auto* const terminal = dynamic_cast<const Terminal*>(*b)) {
        std::cout << terminal->get_name();
      } else {
        const auto* const nonterminal = static_cast<const Nonterminal*>(*b);
        std::cout << nonterminal->get_name();
      }
      std::cout << ' ';
    }
    std::cout << '\n';
  }
}

}  // namespace ast

}  // namespace parser
