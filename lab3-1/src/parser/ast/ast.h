#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "node.h"

namespace parser {

namespace ast {

class SymbolTable;

class ISymbol {
 public:
  virtual ~ISymbol() = default;
};

class SpecialSymbol final : public ISymbol {};  // epsilon or dollar

class NonterminalSymbol final : public ISymbol {
  std::string name_;

 public:
  NonterminalSymbol(std::string name) noexcept : name_(std::move(name)) {}

  const std::string& get_name() const noexcept { return name_; }
};

class TerminalSymbol final : public ISymbol {
  std::string name_;

 public:
  TerminalSymbol(std::string name) noexcept : name_(std::move(name)) {}

  const std::string& get_name() const noexcept { return name_; }
};

class Term final {
  std::vector<const ISymbol*> syms_;

 public:
  Term(std::vector<const ISymbol*>&& syms) noexcept : syms_(std::move(syms)) {}

  auto SymsCbegin() const noexcept { return syms_.cbegin(); }
  auto SymsCend() const noexcept { return syms_.cend(); }
};

class Rule final {
  bool is_axiom_;
  const NonterminalSymbol* lhs_;
  std::vector<std::unique_ptr<Term>> rhs_;

 public:
  Rule(std::vector<std::unique_ptr<Term>>&& rhs,
       const NonterminalSymbol* const lhs, const bool is_axiom) noexcept
      : is_axiom_(is_axiom), lhs_(lhs), rhs_(std::move(rhs)) {}

  bool get_is_axiom() const noexcept { return is_axiom_; }
  const NonterminalSymbol* get_lhs() const noexcept { return lhs_; }
  auto TermsCbegin() const noexcept { return rhs_.cbegin(); }
  auto TermsCend() const noexcept { return rhs_.cend(); }
};

class Program final {
  std::vector<std::unique_ptr<Rule>> rules_;

 public:
  Program(std::vector<std::unique_ptr<Rule>>&& rules) noexcept
      : rules_(std::move(rules)) {}

  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }
};

std::unique_ptr<Program> DtToAst(SymbolTable& symbol_table,
                                 const dt::InnerNode& program);

void Validate(const Program& program);

}  // namespace ast

}  // namespace parser
