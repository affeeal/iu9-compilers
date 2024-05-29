#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// clang-format off
#include <boost/unordered_set.hpp>
// clang-format on

namespace parser {

namespace ast {

class FirstFollow;

class Symbol final {
 public:
  enum class Type {
    kTerminal,
    kNonterminal,
    kSpecial,
  };

 public:
  Symbol(std::string name, const Type type) noexcept
      : name_(std::move(name)), type_(type) {}

  bool operator==(const Symbol&) const = default;

  const std::string& get_name() const noexcept { return name_; }
  Type get_type() const noexcept { return type_; }

 private:
  std::string name_;
  Type type_;
};

const auto kEpsilon = Symbol{"ε", Symbol::Type::kSpecial};
const auto kEndOfProgram = Symbol{"END_OF_PROGRAM", Symbol::Type::kTerminal};

std::size_t hash_value(const Symbol& symbol);

using SymbolVecIter = std::vector<Symbol>::const_iterator;
using SymbolSetIter = boost::unordered_set<Symbol>::const_iterator;

class Term final {
  std::vector<Symbol> symbols_;

 public:
  Term(std::vector<Symbol>&& symbols) noexcept : symbols_(std::move(symbols)) {}

  auto SymbolsCbegin() const noexcept { return symbols_.cbegin(); }
  auto SymbolsCend() const noexcept { return symbols_.cend(); }
};

class Rule final {
  Symbol lhs_;
  std::vector<std::unique_ptr<Term>> rhs_;

 public:
  Rule(Symbol&& lhs, std::vector<std::unique_ptr<Term>>&& rhs) noexcept
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
    assert(rhs_.size() > 0);
  }

  const Symbol& get_lhs() const noexcept { return lhs_; }
  auto TermsCbegin() const noexcept { return rhs_.cbegin(); }
  auto TermsCend() const noexcept { return rhs_.cend(); }
};

class Program final {
  Symbol axiom_;
  std::vector<std::unique_ptr<Rule>> rules_;

 public:
  Program(Symbol&& axiom, std::vector<std::unique_ptr<Rule>>&& rules) noexcept
      : axiom_(std::move(axiom)), rules_(std::move(rules)) {
    Validate();
  }

  const Symbol& get_axiom() const noexcept { return axiom_; }
  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }

 private:
  void Validate() const;
};

}  // namespace ast

}  // namespace parser
