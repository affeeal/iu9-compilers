#include "ast.h"

#include <algorithm>
#include <utility>
#include <vector>

// clang-format off
#include <boost/unordered_set.hpp>
// clang-format on

namespace parser {

namespace ast {

std::size_t hash_value(const Symbol& symbol) {
  std::size_t seed = 0;
  boost::hash_combine(seed, symbol.get_type());
  boost::hash_combine(seed, symbol.get_name());
  return seed;
}

void Program::Validate() const {
  boost::unordered_set<Symbol> defined_nonterminals, involved_nonterminals;
  involved_nonterminals.insert(axiom_);

  for (auto&& rule : rules_) {
    const auto [_, is_inserted] = defined_nonterminals.insert(rule->get_lhs());
    if (!is_inserted) {
      throw std::runtime_error("nonterminal " + rule->get_lhs().get_name() +
                               " redefinition");
    }
  }

  for (auto&& rule : rules_) {
    for (auto b = rule->TermsCbegin(), e = rule->TermsCend(); b != e; ++b) {
      const auto& term = **b;

      for (auto b = term.SymbolsCbegin(), e = term.SymbolsCend(); b != e; ++b) {
        if (b->get_type() != Symbol::Type::kNonTerminal) {
          continue;
        }

        if (!defined_nonterminals.contains(*b)) {
          throw std::runtime_error("undefined nonterminal " + b->get_name());
        }
        involved_nonterminals.insert(*b);
      }
    }
  }

  const auto is_involved =
      [&involved_nonterminals](const std::unique_ptr<Rule>& rule) {
        return involved_nonterminals.contains(rule->get_lhs());
      };

  if (const auto it =
          std::find_if_not(rules_.cbegin(), rules_.cend(), is_involved);
      it != rules_.cend()) {
    throw std::runtime_error("unused nonterminal " +
                             it->get()->get_lhs().get_name());
  }
}

}  // namespace ast

}  // namespace parser
