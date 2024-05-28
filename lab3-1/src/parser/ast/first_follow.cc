#include "first_follow.h"

#include <iostream>

#include "ast.h"

namespace parser {

namespace ast {

FirstFollow::FirstFollow(std::shared_ptr<const Program> program)
    : program_(std::move(program)) {
  BuildFirstSets();
  BuildFollowSets();
}

void FirstFollow::BuildFirstSets() {
  bool sets_are_filling;
  do {
    sets_are_filling = false;

    for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e;
         ++b) {
      const auto& rule = **b;

      auto new_first_set = boost::unordered_set<Symbol>{};
      for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
        const auto& term = **b;

        new_first_set.merge(
            GetFirstSet(term.SymbolsCbegin(), term.SymbolsCend()));
      }

      auto& first_set = first_sets_[rule.get_lhs()];
      if (first_set.size() != new_first_set.size()) {
        sets_are_filling = true;
        first_set = std::move(new_first_set);
      }
    }
  } while (sets_are_filling);
}

boost::unordered_set<Symbol> FirstFollow::GetFirstSet(
    SymbolVecIter b, const SymbolVecIter e) const {
  if (b == e) {
    return {kEpsilon};
  }
  auto new_first_set = boost::unordered_set<Symbol>{};

  for (const auto e_prev = e - 1; b != e; ++b) {
    if (b->get_type() == Symbol::Type::kTerminal) {
      new_first_set.insert(*b);
      break;
    }

    auto first_set = boost::unordered_set<Symbol>{};
    if (const auto it = first_sets_.find(*b); it != first_sets_.cend()) {
      first_set = it->second;
    }

    if (!first_set.contains(kEpsilon)) {
      new_first_set.merge(std::move(first_set));
      break;
    }

    if (b != e_prev) {
      first_set.erase(kEpsilon);
    }
    new_first_set.merge(std::move(first_set));
  }

  return new_first_set;
}

void FirstFollow::BuildFollowSets() {
  follow_sets_[program_->get_axiom()].insert(kDollar);
  auto followed_sets =
      boost::unordered_map<Symbol, boost::unordered_set<Symbol>>{};
  for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e;
       ++b) {
    const auto& rule = **b;

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;
      if (term.SymbolsCbegin() == term.SymbolsCend()) {
        continue;
      }

      const auto e_prev = term.SymbolsCend() - 1;
      for (auto b = term.SymbolsCbegin(), e = e_prev + 1; b != e_prev; ++b) {
        if (b->get_type() != Symbol::Type::kNonterminal) {
          continue;
        }

        auto first_set = GetFirstSet(b + 1, e);
        if (first_set.erase(kEpsilon) && *b != rule.get_lhs()) {
          followed_sets[*b].insert(rule.get_lhs());
        }
        follow_sets_[*b].merge(std::move(first_set));
      }

      if (e_prev->get_type() == Symbol::Type::kNonterminal &&
          *e_prev != rule.get_lhs()) {
        followed_sets[*e_prev].insert(rule.get_lhs());
      }
    }
  }

  bool sets_are_filling;
  do {
    sets_are_filling = false;

    for (auto&& [follower, followed_set] : followed_sets) {
      auto& follow_set = follow_sets_[follower];
      const auto initial_size = follow_set.size();

      for (auto&& nonterminal : followed_set) {
        follow_set.merge(boost::unordered_set{follow_sets_[nonterminal]});
      }

      if (follow_set.size() != initial_size) {
        sets_are_filling = true;
      }
    }
  } while (sets_are_filling);
}

std::pair<SymbolSetIter, SymbolSetIter> FirstFollow::GetFollowSet(
    const Symbol& nonterminal) const {
  const auto& follow_set = follow_sets_.at(nonterminal);
  return {follow_set.cbegin(), follow_set.cend()};
}

void FirstFollow::PrintSets(auto&& sets) const {
  for (auto&& [nonterminal, set] : sets) {
    std::cout << nonterminal.get_name() << ": ";
    for (auto&& symbol : set) {
      std::cout << symbol.get_name() << ' ';
    }
    std::cout << '\n';
  }
}

}  // namespace ast

}  // namespace parser
