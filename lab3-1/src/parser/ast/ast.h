#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace parser {

namespace ast {

class FirstFollow;
class SymbolTable;

class ISymbol {
 public:
  virtual ~ISymbol() = default;

  virtual const std::string& GetName() const = 0;
};

using ISymbolVecIter = std::vector<const ISymbol*>::const_iterator;
using ISymbolSetIter = std::unordered_set<const ISymbol*>::const_iterator;

class Special final : public ISymbol {

};  // epsilon or dollar

class Nonterminal final : public ISymbol {
  std::string name_;

 public:
  Nonterminal(std::string name) noexcept : name_(std::move(name)) {}

  const std::string& get_name() const noexcept { return name_; }
};

class Terminal final : public ISymbol {
  std::string name_;

 public:
  Terminal(std::string name) noexcept : name_(std::move(name)) {}

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
  const Nonterminal* lhs_;
  std::vector<std::unique_ptr<const Term>> rhs_;

 public:
  Rule(std::vector<std::unique_ptr<const Term>>&& rhs,
       const Nonterminal* const lhs) noexcept
      : lhs_(lhs), rhs_(std::move(rhs)) {
    assert(rhs_.size() > 0);
  }

  const Nonterminal* get_lhs() const noexcept { return lhs_; }
  auto TermsCbegin() const noexcept { return rhs_.cbegin(); }
  auto TermsCend() const noexcept { return rhs_.cend(); }
};

class Program final {
  std::vector<std::unique_ptr<const Rule>> rules_;
  std::shared_ptr<const SymbolTable> symbol_table_;
  const Nonterminal* axiom_;

 public:
  Program(std::vector<std::unique_ptr<const Rule>>&& rules,
          std::shared_ptr<const SymbolTable> symbol_table,
          const Nonterminal* const axiom) noexcept;

  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }
  std::shared_ptr<const SymbolTable> get_symbol_table() const noexcept {
    return symbol_table_;
  }
  const Nonterminal* get_axiom() const noexcept { return axiom_; }

 private:
  void Validate() const;
};

}  // namespace ast

}  // namespace parser
