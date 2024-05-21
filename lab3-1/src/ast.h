#pragma once

#include "node.h"

namespace parser {

namespace ast {

class ISymbol {
 public:
  virtual ~ISymbol() = default;
};

class Terminal final : public ISymbol {
  std::string data_;

 public:
  Terminal(const std::string& data) : data_(data) {}
};

class NonTerminal final : public ISymbol {
  std::string data_;

 public:
  NonTerminal(const std::string& data) : data_(data) {}
};

class ITerm {
 public:
  virtual ~ITerm() = default;
};

class Epsilon final : public ITerm {
 public:
  Epsilon() = default;
};

class Term final : public ITerm {
  std::vector<std::unique_ptr<ISymbol>> syms_;

 public:
  Term(std::vector<std::unique_ptr<ISymbol>>&& syms) : syms_(std::move(syms)) {}
};

class Rule final {
  bool is_axiom_;
  NonTerminal lhs_;
  std::vector<std::unique_ptr<ITerm>> rhs_;

 public:
  Rule(std::vector<std::unique_ptr<ITerm>>&& rhs, const NonTerminal lhs,
       const bool is_axiom = false)
      : is_axiom_(is_axiom), lhs_(lhs), rhs_(std::move(rhs)) {}
};

class Program final {
  std::vector<std::unique_ptr<Rule>> rules_;

 public:
  Program(std::vector<std::unique_ptr<Rule>>&& rules)
      : rules_(std::move(rules)) {}
};

std::unique_ptr<Program> DtToAst(const dt::InnerNode& program);

}  // namespace ast

}  // namespace parser
