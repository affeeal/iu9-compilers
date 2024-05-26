#include "first_follow.h"

#include "ast.h"
#include "symbol_table.h"

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
  const auto& symbol_table = program_->get_symbol_table();

  assert(e > b);
  if (*b == symbol_table.GetEpsilon()) {
    return {symbol_table.GetEpsilon()};
  }
  auto new_first_set = std::unordered_set<const ISymbol*>{};

  for (const auto e_prev = e - 1; b != e; ++b) {
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

    if (!first_set.contains(symbol_table.GetEpsilon())) {
      new_first_set.merge(std::move(first_set));
      break;
    }

    if (b != e_prev) {
      first_set.erase(symbol_table.GetEpsilon());
    }
    new_first_set.merge(std::move(first_set));
  }

  return new_first_set;
}

void FirstFollow::BuildFollowSets() {
  const auto& symbol_table = program_->get_symbol_table();
  auto followed_sets =
      std::unordered_map<const Nonterminal*,
                         std::unordered_set<const Nonterminal*>>{};

  for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e;
       ++b) {
    const auto& rule = **b;

    if (rule.get_is_axiom()) {
      follow_sets_[rule.get_lhs()].insert(symbol_table.GetDollar());
    }

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;

      for (auto b = term.SymsCbegin(), e = term.SymsCend(), e_prev = e - 1;
           b != e_prev; ++b) {
        const auto* const nonterminal = dynamic_cast<const Nonterminal*>(*b);
        if (!nonterminal) {
          continue;
        }

        auto first_set = GetFirstSet(b + 1, e);
        if (first_set.erase(symbol_table.GetEpsilon()) &&
            nonterminal != rule.get_lhs()) {
          followed_sets[nonterminal].insert(rule.get_lhs());
        }
        follow_sets_[nonterminal].merge(std::move(first_set));
      }

      const auto* const nonterminal =
          dynamic_cast<const Nonterminal*>(*(term.SymsCend() - 1));
      if (nonterminal && nonterminal != rule.get_lhs()) {
        followed_sets[nonterminal].insert(rule.get_lhs());
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
        follow_set.merge(std::unordered_set{follow_sets_[nonterminal]});
      }

      if (follow_set.size() != initial_size) {
        sets_are_filling = true;
      }
    }
  } while (sets_are_filling);
}

std::pair<ISymbolSetIter, ISymbolSetIter> FirstFollow::GetFollowSet(
    const Nonterminal* const nonterminal) const {
  const auto& follow_set = follow_sets_.at(nonterminal);
  return {follow_set.cbegin(), follow_set.cend()};
}

}  // namespace ast

}  // namespace parser
