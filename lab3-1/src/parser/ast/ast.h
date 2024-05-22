#pragma once

#include "node.h"
#include "visitor.h"

namespace parser {

namespace ast {

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
  std::string lhs_;
  std::vector<std::unique_ptr<Term>> rhs_;

 public:
  Rule(std::string lhs, std::vector<std::unique_ptr<Term>>&& rhs,
       const bool is_axiom) noexcept
      : is_axiom_(is_axiom), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  const std::string& get_lhs() const noexcept { return lhs_; }
  bool get_is_axiom() const noexcept { return is_axiom_; }

  auto RhsCbegin() const noexcept { return rhs_.cbegin(); }
  auto RhsCend() const noexcept { return rhs_.cend(); }

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
