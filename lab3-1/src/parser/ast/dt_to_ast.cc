#include "dt_to_ast.h"

#include <algorithm>
#include <iterator>

#include "ast.h"

namespace parser {

namespace ast {

std::shared_ptr<Program> DtToAst::Convert(const dt::InnerNode& program) {
  axiom_ = nullptr;
  return ParseProgram(program);
}

// Program ::= Rules
std::unique_ptr<Program> DtToAst::ParseProgram(const dt::InnerNode& program) {
  const auto& rules =
      static_cast<const dt::InnerNode&>(**program.ChildrenCbegin());
  auto ast_rules = ParseRules(rules);

  if (!axiom_) {
    throw std::runtime_error("axiom is not defined");
  }
  return std::make_unique<Program>(std::move(*axiom_), std::move(ast_rules));
}

// Rules ::= Rule Rules | ε
std::vector<std::unique_ptr<Rule>> DtToAst::ParseRules(
    const dt::InnerNode& rules) {
  const auto b = rules.ChildrenCbegin();
  if (b == rules.ChildrenCend()) {
    return {};  // TODO: fix to contain at least one rule
  }

  const auto& rule = static_cast<const dt::InnerNode&>(**b);
  auto ast_rule = ParseRule(rule);

  const auto& rules_rhs = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_rules = ParseRules(rules_rhs);

  ast_rules.push_back(std::move(ast_rule));
  std::rotate(ast_rules.rbegin(), ast_rules.rbegin() + 1, ast_rules.rend());
  return ast_rules;
}

// Rule ::= RuleLHS ARROW RuleRHS
std::unique_ptr<Rule> DtToAst::ParseRule(const dt::InnerNode& rule) {
  const auto b = rule.ChildrenCbegin();

  const auto& rule_lhs = static_cast<const dt::InnerNode&>(**b);
  auto lhs = ParseRuleLHS(rule_lhs);

  const auto& rule_rhs = static_cast<const dt::InnerNode&>(**(b + 2));
  auto rhs = ParseRuleRHS(rule_rhs);

  return std::make_unique<Rule>(std::move(lhs), std::move(rhs));
}

// RuleLHS ::= KW_AXIOM NONTERMINAL | NONTERMINAL
Symbol DtToAst::ParseRuleLHS(const dt::InnerNode& rule_lhs) {
  const auto b = rule_lhs.ChildrenCbegin();
  if (rule_lhs.ChildrenCend() - b == 2) {
    const auto& leaf = static_cast<const dt::LeafNode&>(**(b + 1));
    const auto* const nonterminal =
        static_cast<const lexer::NonterminalToken*>(leaf.get_token());

    if (axiom_) {
      nonterminal->ThrowError("axiom redefinition");
    }
    axiom_ = std::make_unique<Symbol>(nonterminal->get_str(),
                                      Symbol::Type::kNonTerminal);
    return *axiom_;
  }

  const auto& leaf = static_cast<const dt::LeafNode&>(**b);
  const auto* const nonterminal =
      static_cast<const lexer::NonterminalToken*>(leaf.get_token());
  return {nonterminal->get_str(), Symbol::Type::kNonTerminal};
}

// RuleRHS ::= Expr KW_END
std::vector<std::unique_ptr<Term>> DtToAst::ParseRuleRHS(
    const dt::InnerNode& rule_rhs) {
  const auto& expr =
      static_cast<const dt::InnerNode&>(**rule_rhs.ChildrenCbegin());
  return ParseExpr(expr);
}

// Expr ::= Term Expr1
std::vector<std::unique_ptr<Term>> DtToAst::ParseExpr(
    const dt::InnerNode& expr) {
  const auto b = expr.ChildrenCbegin();

  const auto& term = static_cast<const dt::InnerNode&>(**b);
  auto ast_term = ParseTerm(term);

  const auto& expr1 = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_expr1 = ParseExpr1(expr1);

  ast_expr1.push_back(std::move(ast_term));
  std::rotate(ast_expr1.rbegin(), ast_expr1.rbegin() + 1, ast_expr1.rend());
  return ast_expr1;
}

// Expr1 ::= KW_OR Term Expr1 | ε
std::vector<std::unique_ptr<Term>> DtToAst::ParseExpr1(
    const dt::InnerNode& expr1) {
  const auto b = expr1.ChildrenCbegin();
  if (b == expr1.ChildrenCend()) {
    return {};
  }

  const auto& term = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term = ParseTerm(term);

  const auto& expr1_rhs = static_cast<const dt::InnerNode&>(**(b + 2));
  auto ast_expr1 = ParseExpr1(expr1_rhs);

  ast_expr1.push_back(std::move(ast_term));
  std::rotate(ast_expr1.rbegin(), ast_expr1.rbegin() + 1, ast_expr1.rend());
  return ast_expr1;
}

// Term ::= Symbol Term1 | KW_EPSILON
std::unique_ptr<Term> DtToAst::ParseTerm(const dt::InnerNode& term) {
  const auto b = term.ChildrenCbegin();
  if (term.ChildrenCend() - b == 1) {
    return std::make_unique<Term>(std::vector<Symbol>{});
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  auto ast_symbol = ParseSymbol(symbol);

  const auto& term1 = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(term1);

  ast_term1.push_back(std::move(ast_symbol));
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return std::make_unique<Term>(std::move(ast_term1));
}

// Term1 ::= Symbol Term1 | ε
std::vector<Symbol> DtToAst::ParseTerm1(const dt::InnerNode& term1) {
  const auto b = term1.ChildrenCbegin();
  if (b == term1.ChildrenCend()) {
    return {};
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  auto ast_symbol = ParseSymbol(symbol);

  const auto& term1_rhs = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(term1_rhs);

  ast_term1.push_back(std::move(ast_symbol));
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return ast_term1;
}

// Symbol ::= TERMINAL | NONTERMINAL
Symbol DtToAst::ParseSymbol(const dt::InnerNode& symbol) {
  const auto& leaf =
      static_cast<const dt::LeafNode&>(**symbol.ChildrenCbegin());

  if (const auto* const terminal =
          dynamic_cast<const lexer::TerminalToken*>(leaf.get_token())) {
    return {terminal->get_str(), Symbol::Type::kTerminal};
  }

  const auto& nonterminal =
      static_cast<const lexer::NonterminalToken&>(*leaf.get_token());
  return {nonterminal.get_str(), Symbol::Type::kNonTerminal};
}

}  // namespace ast

}  // namespace parser
