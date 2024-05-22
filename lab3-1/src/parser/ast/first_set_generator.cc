#include "first_set_generator.h"

#include <unordered_set>

#include "ast.h"

namespace parser {

namespace ast {

static const std::string kEpsilon = "";

void FirstSetGenerator::Visit(const Program& program) {
  do {
    sets_are_changing_ = false;
    for (auto f = program.RulesCbegin(), l = program.RulesCend(); f != l; ++f) {
      f->get()->Accept(*this);
      auto& first_set = first_sets_[f->get()->get_lhs()];
      if (return_.size() != first_set.size()) {
        sets_are_changing_ = true;
        first_set = std::move(return_);
      }
    }
  } while (sets_are_changing_);
}

void FirstSetGenerator::Visit(const Rule& rule) {
  auto first_set = std::unordered_set<std::string>{};
  for (auto f = rule.RhsCbegin(), l = rule.RhsCend(); f != l; ++f) {
    f->get()->Accept(*this);
    first_set.merge(std::move(return_));
  }
  return_ = std::move(first_set);
}

void FirstSetGenerator::Visit(const Term& term) {
  if (term.SymsCbegin() == term.SymsCend()) {
    return_ = {kEpsilon};
    return;
  }

  auto first_set = std::unordered_set<std::string>{};
  for (auto f = term.SymsCbegin(), l = term.SymsCend() - 1; f != l; ++f) {
    f->Accept(*this);
    if (!return_.contains(kEpsilon)) {
      return_.merge(std::move(first_set));
      return;
    }
    return_.erase(kEpsilon);
    first_set.merge(std::move(return_));
  }

  (term.SymsCend() - 1)->Accept(*this);
  return_.merge(std::move(first_set));
}

void FirstSetGenerator::Visit(const Symbol& symbol) {
  switch (symbol.get_type()) {
    case Symbol::Type::kTerminal: {
      return_ = {symbol.get_name()};
      break;
    }
    case Symbol::Type::kNonterminal: {
      return_ = first_sets_.at(symbol.get_name());
      break;
    }
  }
}

}  // namespace ast

}  // namespace parser
