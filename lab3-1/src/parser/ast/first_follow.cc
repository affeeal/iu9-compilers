/*
#include "first_follow.h"

#include <iostream>

#include "ast.h"

namespace parser {

namespace ast {

FirstFollow::FirstFollow(const Program& program) {
  BuildFirstSets(program);
  BuildFollowSets(program);

  PrintSets(follow_sets_);
}

void FirstFollow::BuildFirstSets(const Program& program) {
  bool sets_are_filling;

  do {
    sets_are_filling = false;

    for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
      const auto& rule = **b;

      auto new_first_set = std::unordered_set<TableSymbol>{};
      for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
        const auto& term = **b;

        new_first_set.merge(GetFirstSet(term.SymsCbegin(), term.SymsCend()));
      }

      auto& first_set = first_sets_[rule.get_name()];
      if (first_set.size() != new_first_set.size()) {
        sets_are_filling = true;
        first_set = std::move(new_first_set);
      }
    }
  } while (sets_are_filling);
}

std::unordered_set<TableSymbol> FirstFollow::GetFirstSet(
    SymbolVectorIter b, const SymbolVectorIter e) const {
  if (b == e) {
    return {SpecialSymbol::kEpsilon};
  }

  auto new_first_set = std::unordered_set<TableSymbol>{};
  for (const auto e_prev = e - 1; b != e; ++b) {
    if (b->get_type() == Symbol::Type::kTerminal) {
      new_first_set.insert(b->get_name());
      break;
    }

    // b->get_type() == Symbol::Type::kNonterminal

    auto first_set = std::unordered_set<TableSymbol>{};
    if (const auto it = first_sets_.find(b->get_name());
        it != first_sets_.cend()) {
      first_set = it->second;
    }

    if (!first_set.contains(SpecialSymbol::kEpsilon)) {
      new_first_set.merge(std::move(first_set));
      break;
    }

    if (b != e_prev) {
      first_set.erase(SpecialSymbol::kEpsilon);
    }
    new_first_set.merge(std::move(first_set));
  }

  return new_first_set;
}

void FirstFollow::PrintSets(auto&& sets) const {
  for (auto&& [name, set] : sets) {
    std::cout << name << ": ";
    for (auto&& symbol : set) {
      std::cout << symbol << ' ';
    }
    std::cout << '\n';
  }
}

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

}  // namespace ast

}  // namespace parser
   // */
