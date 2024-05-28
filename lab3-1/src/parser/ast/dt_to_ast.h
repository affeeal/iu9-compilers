#pragma once

#include "node.h"
#include "symbol_table.h"

namespace parser {

namespace ast {

class DtToAst final {
 public:
  std::shared_ptr<const Program> Convert(const dt::InnerNode& program);

 private:
  void Reset() noexcept;

  std::unique_ptr<const Program> ParseProgram(const dt::InnerNode& program);
  std::vector<std::unique_ptr<const Rule>> ParseRules(
      const dt::InnerNode& rules);
  std::unique_ptr<const Rule> ParseRule(const dt::InnerNode& rule);
  const Nonterminal* ParseRuleLHS(const dt::InnerNode& rule_lhs);
  std::vector<std::unique_ptr<const Term>> ParseRuleRHS(
      const dt::InnerNode& rule_rhs);
  std::vector<std::unique_ptr<const Term>> ParseExpr(const dt::InnerNode& expr);
  std::vector<std::unique_ptr<const Term>> ParseExpr1(
      const dt::InnerNode& expr1);
  std::unique_ptr<const Term> ParseTerm(const dt::InnerNode& term);
  std::vector<const ISymbol*> ParseTerm1(const dt::InnerNode& term1);
  const ISymbol* ParseSymbol(const dt::InnerNode& symbol);

 private:
  SymbolTable symbol_table_;
  const Nonterminal* axiom_;
};

}  // namespace ast

}  // namespace parser
