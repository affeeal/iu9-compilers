#pragma once

#include <unordered_map>

#include "node.h"
#include "visitor.h"

namespace parser {

namespace ast {

enum class Special {
  kEpsilon,
  kDollar,
};

std::string_view ToString(const Special symbol);

using TableSymbol = std::variant<std::string, Special>;
std::ostream& operator<<(std::ostream& os, const TableSymbol& symbol);

struct Index final {
  const Rule* axiom = nullptr;
  std::unordered_map<std::string, const Rule*> rules;
};

class INode {
 public:
  virtual ~INode() = default;

  virtual void Accept(IVisitor& visitor) const = 0;
};

class Symbol final : public INode {
 public:
  enum class Type {
    kTerminal,
    kNonterminal,
  };

 public:
  Symbol(std::string name, const Type type) noexcept
      : type_(type), name_(std::move(name)) {}

  const std::string& get_name() const noexcept { return name_; }
  Type get_type() const noexcept { return type_; }

  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }

 private:
  Type type_;
  std::string name_;
};

class Term final : public INode {
  std::vector<Symbol> syms_;

 public:
  Term(std::vector<Symbol>&& syms) noexcept : syms_(std::move(syms)) {}

  auto SymsCbegin() const noexcept { return syms_.cbegin(); }
  auto SymsCend() const noexcept { return syms_.cend(); }

  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }
};

class Rule final : public INode {
  bool is_axiom_;
  std::string name_;
  std::vector<std::unique_ptr<Term>> terms_;

 public:
  Rule(std::string name, std::vector<std::unique_ptr<Term>>&& terms,
       const bool is_axiom) noexcept
      : is_axiom_(is_axiom), name_(std::move(name)), terms_(std::move(terms)) {}

  const std::string& get_name() const noexcept { return name_; }
  bool get_is_axiom() const noexcept { return is_axiom_; }

  auto TermsCbegin() const noexcept { return terms_.cbegin(); }
  auto TermsCend() const noexcept { return terms_.cend(); }

  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }
};

class Program final : public INode {
  std::vector<std::unique_ptr<Rule>> rules_;

 public:
  Program(std::vector<std::unique_ptr<Rule>>&& rules) noexcept
      : rules_(std::move(rules)) {}

  auto RulesCbegin() const noexcept { return rules_.cbegin(); }
  auto RulesCend() const noexcept { return rules_.cend(); }

  void Accept(IVisitor& visitor) const override { visitor.Visit(*this); }
};

std::unique_ptr<Program> DtToAst(const dt::InnerNode& program);

}  // namespace ast

}  // namespace parser
