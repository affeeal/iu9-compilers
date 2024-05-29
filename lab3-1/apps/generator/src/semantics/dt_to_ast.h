#pragma once

#include "ast.h"
#include "parser.h"

namespace semantics {

class DtToAst final {
 public:
  std::shared_ptr<Program> Convert(const parser::InnerNode& program);

 private:
  std::unique_ptr<Program> ParseProgram(const parser::InnerNode& program);
  std::vector<std::unique_ptr<Rule>> ParseRules(const parser::InnerNode& rules);
  std::unique_ptr<Rule> ParseRule(const parser::InnerNode& rule);
  parser::Symbol ParseRuleLHS(const parser::InnerNode& rule_lhs);
  std::vector<std::unique_ptr<Term>> ParseRuleRHS(
      const parser::InnerNode& rule_rhs);
  std::vector<std::unique_ptr<Term>> ParseExpr(const parser::InnerNode& expr);
  std::vector<std::unique_ptr<Term>> ParseExpr1(const parser::InnerNode& expr1);
  std::unique_ptr<Term> ParseTerm(const parser::InnerNode& term);
  std::vector<parser::Symbol> ParseTerm1(const parser::InnerNode& term1);
  parser::Symbol ParseSymbol(const parser::InnerNode& symbol);

 private:
  std::unique_ptr<parser::Symbol> axiom_;
};

}  // namespace semantics
