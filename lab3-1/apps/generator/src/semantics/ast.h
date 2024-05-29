#pragma once

#include "symbol.h"

namespace semantics {

class Term final {
  std::vector<parser::Symbol> symbols_;

 public:
  Term(std::vector<parser::Symbol>&& symbols) noexcept
      : symbols_(std::move(symbols)) {}

  auto SymbolsCbegin() const noexcept { return symbols_.cbegin(); }
  auto SymbolsCend() const noexcept { return symbols_.cend(); }
};

class Rule final {
  parser::Symbol lhs_;
  std::vector<std::unique_ptr<Term>> rhs_;

 public:
  Rule(parser::Symbol&& lhs, std::vector<std::unique_ptr<Term>>&& rhs) noexcept
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
    assert(rhs_.size() > 0);
  }

  const parser::Symbol& get_lhs() const noexcept { return lhs_; }
  auto TermsCbegin() const noexcept { return rhs_.cbegin(); }
  auto TermsCend() const noexcept { return rhs_.cend(); }
};

class Program final {
  parser::Symbol axiom_;
  std::vector<std::unique_ptr<Rule>> rules_;

 public:
  Program(parser::Symbol&& axiom,
          std::vector<std::unique_ptr<Rule>>&& rules) noexcept
      : axiom_(std::move(axiom)), rules_(std::move(rules)) {
    Validate();
  }

  const parser::Symbol& get_axiom() const noexcept { return axiom_; }
  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }

 private:
  void Validate() const;
};

}  // namespace semantics
