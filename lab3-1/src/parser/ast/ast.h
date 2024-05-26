#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include "node.h"

namespace parser {

namespace ast {

class FirstFollow;
class SymbolTable;

class ISymbol {
 public:
  virtual ~ISymbol() = default;
};

class Special final : public ISymbol {};  // epsilon or dollar

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
  Term(std::vector<const ISymbol*>&& syms) noexcept : syms_(std::move(syms)) {
    assert(syms_.size() > 0);
  }

  auto SymsCbegin() const noexcept { return syms_.cbegin(); }
  auto SymsCend() const noexcept { return syms_.cend(); }
};

class Rule final {
  bool is_axiom_;
  const Nonterminal* lhs_;
  std::vector<std::unique_ptr<const Term>> rhs_;

 public:
  Rule(std::vector<std::unique_ptr<const Term>>&& rhs,
       const Nonterminal* const lhs, const bool is_axiom) noexcept
      : is_axiom_(is_axiom), lhs_(lhs), rhs_(std::move(rhs)) {
    assert(rhs_.size() > 0);
  }

  bool get_is_axiom() const noexcept { return is_axiom_; }
  const Nonterminal* get_lhs() const noexcept { return lhs_; }
  auto TermsCbegin() const noexcept { return rhs_.cbegin(); }
  auto TermsCend() const noexcept { return rhs_.cend(); }
};

class Program final {
  std::vector<std::unique_ptr<const Rule>> rules_;
  std::unique_ptr<const SymbolTable> symbol_table_;

 public:
  Program(std::vector<std::unique_ptr<const Rule>>&& rules,
          std::unique_ptr<const SymbolTable>&& symbol_table) noexcept;

  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }

  const SymbolTable& get_symbol_table() const noexcept {
    return *symbol_table_;
  }
};

std::shared_ptr<const Program> DtToAst(const dt::InnerNode& program);

void Validate(const Program& program);

using TableKey = std::pair<const Nonterminal*, const ISymbol*>;
std::unordered_map<TableKey, std::vector<const ISymbol*>, boost::hash<TableKey>>
BuildTable(const FirstFollow& first_follow);

}  // namespace ast

}  // namespace parser
