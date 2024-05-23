#include "first_set_generator.h"

#include <iostream>
#include <variant>

#include "ast.h"

namespace parser {

namespace ast {

FirstFollow::FirstFollow(const Program& program) {
  BuildFirstSets(program);
}

void FirstFollow::BuildFirstSets(const Program& program) {
  bool sets_are_changing;
  
  do {
    sets_are_changing = false;
    
    for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
      const auto& rule = **b;

      auto new_first_set = std::unordered_set<TableSymbol>{};
      for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
        const auto& term = **b;

        new_first_set.merge(GetFirstSet(term.SymsCbegin(), term.SymsCend()));
      }

      auto& first_set = first_sets_[rule.get_name()];
      if (first_set.size() != new_first_set.size()) {
        sets_are_changing = true;
        first_set = std::move(new_first_set);
      }
    }
  } while (sets_are_changing);

  PrintFirstSets();
}

std::unordered_set<TableSymbol> FirstFollow::GetFirstSet(
    FirstFollow::SymbolIter b, const FirstFollow::SymbolIter e) {
  if (b == e) {
    return {Special::kEpsilon};
  }

  auto new_first_set = std::unordered_set<TableSymbol>{};
  for (const auto e_prev = e - 1; b != e; ++b) {
    if (b->get_type() == Symbol::Type::kTerminal) {
      new_first_set.insert(b->get_name());
      break;
    }

    // b->get_type() == Symbol::Type::kNonterminal
    auto first_set = first_sets_.at(b->get_name());
    if (!first_set.contains(Special::kEpsilon)) {
      new_first_set.merge(std::move(first_set));
      break;
    }

    if (b != e_prev) {
      first_set.erase(Special::kEpsilon);
    }
    new_first_set.merge(std::move(first_set));
  }

  return new_first_set;
}

void FirstFollow::PrintFirstSets() const {
  std::cout << "First sets:\n";
  
  for (auto&& [name, first_set] : first_sets_) {
    std::cout << name << ": ";

    for (auto&& symbol : first_set) {
      if (const auto* const special = std::get_if<Special>(&symbol)) {
        std::cout << ToString(*special);
      } else {
        std::cout << std::get<std::string>(symbol);
      }

      std::cout << ' ';
    }

    std::cout << '\n';
  }
}

}  // namespace ast

}  // namespace parser
