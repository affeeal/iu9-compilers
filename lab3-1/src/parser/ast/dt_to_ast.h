#pragma once

#include "ast.h"
#include "node.h"

namespace parser {

namespace ast {

class DtToAst final {
 public:
  std::shared_ptr<Program> Convert(const dt::InnerNode& program);

 private:
  std::unique_ptr<Program> ParseProgram(const dt::InnerNode& program);
  std::vector<std::unique_ptr<Rule>> ParseRules(const dt::InnerNode& rules);
  std::unique_ptr<Rule> ParseRule(const dt::InnerNode& rule);
  Symbol ParseRuleLHS(const dt::InnerNode& rule_lhs);
  std::vector<std::unique_ptr<Term>> ParseRuleRHS(
      const dt::InnerNode& rule_rhs);
  std::vector<std::unique_ptr<Term>> ParseExpr(const dt::InnerNode& expr);
  std::vector<std::unique_ptr<Term>> ParseExpr1(const dt::InnerNode& expr1);
  std::unique_ptr<Term> ParseTerm(const dt::InnerNode& term);
  std::vector<Symbol> ParseTerm1(const dt::InnerNode& term1);
  Symbol ParseSymbol(const dt::InnerNode& symbol);

 private:
  std::unique_ptr<Symbol> axiom_;
};

}  // namespace ast

}  // namespace parser
