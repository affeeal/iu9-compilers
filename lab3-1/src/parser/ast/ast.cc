#include "ast.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "first_follow.h"
#include "symbol_table.h"

namespace parser {

namespace ast {

Program::Program(std::vector<std::unique_ptr<const Rule>>&& rules,
                 std::shared_ptr<const SymbolTable> symbol_table,
                 const Nonterminal* const axiom) noexcept
    : rules_(std::move(rules)),
      symbol_table_(std::move(symbol_table)),
      axiom_(axiom) {
  Validate();
}

void Program::Validate() const {
  assert(axiom_);
  assert(rules_.size() > 0);

  std::unordered_set<const Nonterminal*> defined_nonterminals,
      involved_nonterminals;
  involved_nonterminals.insert(axiom_);

  for (auto&& rule : rules_) {
    if (const auto [_, is_inserted] =
            defined_nonterminals.insert(rule->get_lhs());
        !is_inserted) {
      throw std::runtime_error("nonterminal " + rule->get_lhs()->get_name() +
                               " redefinition");
    }
  }

  for (auto&& rule : rules_) {
    for (auto b = rule->TermsCbegin(), e = rule->TermsCend(); b != e; ++b) {
      const auto& term = **b;

      for (auto b = term.SymsCbegin(), e = term.SymsCend(); b != e; ++b) {
        const auto* const nonterminal = dynamic_cast<const Nonterminal*>(*b);
        if (!nonterminal) {
          continue;
        }

        if (!defined_nonterminals.contains(nonterminal)) {
          throw std::runtime_error("undefined nonterminal " +
                                   nonterminal->get_name());
        }
        involved_nonterminals.insert(nonterminal);
      }
    }
  }

  const auto is_involved =
      [&involved_nonterminals](const std::unique_ptr<const Rule>& rule) {
        return involved_nonterminals.contains(rule->get_lhs());
      };

  if (const auto it =
          std::find_if_not(rules_.cbegin(), rules_.cend(), is_involved);
      it != rules_.cend()) {
    throw std::runtime_error("unused nonterminal " +
                             it->get()->get_lhs()->get_name());
  }
}

}  // namespace ast

}  // namespace parser
