#include "first_follow.h"

#include <iostream>

#include "ast.h"

namespace parser {

namespace ast {

FirstFollow::FirstFollow(std::shared_ptr<const SymbolTable> symbol_table,
                         const Program& program)
    : symbol_table_(std::move(symbol_table)) {
  BuildFirstSets(program);
  // BuildFollowSets(program);

  PrintSets(first_sets_);
}

void FirstFollow::BuildFirstSets(const Program& program) {
  bool sets_are_filling;
  do {
    sets_are_filling = false;

    for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
      const auto& rule = **b;

      auto new_first_set = std::unordered_set<const ISymbol*>{};
      for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
        const auto& term = **b;

        new_first_set.merge(GetFirstSet(term.SymsCbegin(), term.SymsCend()));
      }

      auto& first_set = first_sets_[rule.get_lhs()];
      if (first_set.size() != new_first_set.size()) {
        sets_are_filling = true;
        first_set = std::move(new_first_set);
      }
    }
  } while (sets_are_filling);
}

std::unordered_set<const ISymbol*> FirstFollow::GetFirstSet(
    ISymbolVecIter b, const ISymbolVecIter e) const {
  if (e - b == 1 && *b == symbol_table_->GetEpsilon()) {
    return {symbol_table_->GetEpsilon()};
  }
  auto new_first_set = std::unordered_set<const ISymbol*>{};

  const auto e_prev = e - 1;
  for (; b != e; ++b) {
    if (const auto* const terminal = dynamic_cast<const Terminal*>(*b)) {
      new_first_set.insert(terminal);
      break;
    }
    const auto* const nonterminal = static_cast<const Nonterminal*>(*b);

    auto first_set = std::unordered_set<const ISymbol*>{};
    if (const auto it = first_sets_.find(nonterminal);
        it != first_sets_.cend()) {
      first_set = it->second;
    }

    if (!first_set.contains(symbol_table_->GetEpsilon())) {
      new_first_set.merge(std::move(first_set));
      break;
    }

    if (b != e_prev) {
      first_set.erase(symbol_table_->GetEpsilon());
    }
    new_first_set.merge(std::move(first_set));
  }

  return new_first_set;
}

void FirstFollow::PrintSets(auto&& sets) const {
  for (auto&& [nonterminal, set] : sets) {
    std::cout << nonterminal->get_name() << ": ";
    for (auto&& symbol : set) {
      if (const auto* const terminal = dynamic_cast<const Terminal*>(symbol)) {
        std::cout << terminal->get_name();
      } else if (symbol == symbol_table_->GetEpsilon()) {
        std::cout << "epsilon";
      }
      std::cout << ' ';
    }
    std::cout << '\n';
  }
}

/*
void FirstFollow::BuildFollowSets(const Program& program) {
  auto followed =
      std::unordered_map<std::string, std::unordered_set<std::string>>{};

  for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
    const auto& rule = **b;

    if (rule.get_is_axiom()) {
      follow_sets_[rule.get_name()].insert(SpecialSymbol::kDollar);
    }

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;
      if (term.SymsCbegin() == term.SymsCend()) {
        continue;
      }

      const auto e_prev = term.SymsCend() - 1;
      for (auto b = term.SymsCbegin(), e = term.SymsCend(); b != e_prev; ++b) {
        const auto& symbol = *b;
        if (symbol.get_type() == Symbol::Type::kTerminal) {
          continue;
        }

        auto first_set = GetFirstSet(b + 1, e);
        if (first_set.erase(SpecialSymbol::kEpsilon) &&
            symbol.get_name() != rule.get_name()) {
          followed[symbol.get_name()].insert(rule.get_name());
        }
        follow_sets_[symbol.get_name()].merge(std::move(first_set));
      }

      if (e_prev->get_type() == Symbol::Type::kNonterminal &&
          e_prev->get_name() != rule.get_name()) {
        followed[e_prev->get_name()].insert(rule.get_name());
      }
    }
  }

  bool sets_are_filling;
  do {
    sets_are_filling = false;

    for (auto&& [follower, names] : followed) {
      auto& follow_set = follow_sets_[follower];
      const auto initial_size = follow_set.size();

      for (auto&& name : names) {
        follow_set.merge(std::unordered_set{follow_sets_[name]});
      }

      if (follow_set.size() != initial_size) {
        sets_are_filling = true;
      }
    }
  } while (sets_are_filling);
}

std::pair<TableSymbolSetIter, TableSymbolSetIter> FirstFollow::GetFollowSet(
    const std::string& name) const {
  const auto& follow_set = follow_sets_.at(name);
  return {follow_set.cbegin(), follow_set.cend()};
}
*/

}  // namespace ast

}  // namespace parser
