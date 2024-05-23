#include "ast.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include "node.h"
#include "token.h"

namespace parser {

namespace ast {

std::string_view ToString(const Special symbol) {
  switch (symbol) {
    case Special::kEpsilon: {
      return "ε";
    }
    case Special::kDollar: {
      return "$";
    }
  }
}

// Symbol ::= TERMINAL | NONTERMINAL
Symbol ParseSymbol(const dt::InnerNode& symbol) {
  const auto& leaf =
      static_cast<const dt::LeafNode&>(**symbol.ChildrenCbegin());
  if (const auto* const terminal =
          dynamic_cast<const lexer::TerminalToken*>(leaf.get_token())) {
    return Symbol{terminal->get_str(), Symbol::Type::kTerminal};
  }

  const auto* const nonterminal =
      static_cast<const lexer::NonterminalToken*>(leaf.get_token());
  return Symbol{nonterminal->get_str(), Symbol::Type::kNonterminal};
}

// Term1 ::= Symbol Term1 | ε
std::vector<Symbol> ParseTerm1(const dt::InnerNode& term1) {
  const auto b = term1.ChildrenCbegin();
  if (b == term1.ChildrenCend()) {
    return {};
  }

  auto symbol = ParseSymbol(static_cast<const dt::InnerNode&>(**b));
  auto term1_rhs = ParseTerm1(static_cast<const dt::InnerNode&>(**(b + 1)));

  term1_rhs.push_back(std::move(symbol));
  std::rotate(term1_rhs.rbegin(), term1_rhs.rbegin() + 1, term1_rhs.rend());
  return term1_rhs;
}

// Term ::= Symbol Term1 | KW_EPSILON
std::unique_ptr<Term> ParseTerm(const dt::InnerNode& term) {
  const auto b = term.ChildrenCbegin();
  if (term.ChildrenCend() - b == 1) {
    return std::make_unique<Term>(std::vector<Symbol>{});
  }

  auto symbol = ParseSymbol(static_cast<const dt::InnerNode&>(**b));
  auto term1 = ParseTerm1(static_cast<const dt::InnerNode&>(**(b + 1)));

  term1.push_back(std::move(symbol));
  std::rotate(term1.rbegin(), term1.rbegin() + 1, term1.rend());
  return std::make_unique<Term>(std::move(term1));
}

// Expr1 ::= KW_OR Term Expr1 | ε
std::vector<std::unique_ptr<Term>> ParseExpr1(const dt::InnerNode& expr1) {
  const auto b = expr1.ChildrenCbegin();
  if (b == expr1.ChildrenCend()) {
    return {};
  }

  auto term = ParseTerm(static_cast<const dt::InnerNode&>(**(b + 1)));
  auto expr1_rhs = ParseExpr1(static_cast<const dt::InnerNode&>(**(b + 2)));

  expr1_rhs.push_back(std::move(term));
  std::rotate(expr1_rhs.rbegin(), expr1_rhs.rbegin() + 1, expr1_rhs.rend());
  return expr1_rhs;
}

// Expr ::= Term Expr1
std::vector<std::unique_ptr<Term>> ParseExpr(const dt::InnerNode& expr) {
  const auto b = expr.ChildrenCbegin();
  auto term = ParseTerm(static_cast<const dt::InnerNode&>(**b));
  auto expr1 = ParseExpr1(static_cast<const dt::InnerNode&>(**(b + 1)));

  expr1.push_back(std::move(term));
  std::rotate(expr1.rbegin(), expr1.rbegin() + 1, expr1.rend());
  return expr1;
}

// RuleRHS ::= Expr KW_END
std::vector<std::unique_ptr<Term>> ParseRuleRHS(const dt::InnerNode& rule_rhs) {
  const auto b = rule_rhs.ChildrenCbegin();
  return ParseExpr(static_cast<const dt::InnerNode&>(**b));
}

// RuleLHS ::= KW_AXIOM NONTERMINAL | NONTERMINAL
std::pair<std::string, bool> ParseRuleLHS(const dt::InnerNode& rule_lhs) {
  const auto b = rule_lhs.ChildrenCbegin();
  if (rule_lhs.ChildrenCend() - b == 2) {
    const auto& leaf = static_cast<const dt::LeafNode&>(**(b + 1));
    const auto* const nonterminal =
        static_cast<const lexer::NonterminalToken*>(leaf.get_token());
    return {nonterminal->get_str(), true};
  }

  const auto& leaf = static_cast<const dt::LeafNode&>(**b);
  const auto* const nonterminal =
      static_cast<const lexer::NonterminalToken*>(leaf.get_token());
  return {nonterminal->get_str(), false};
}

// Rule ::= RuleLHS OP_ARROW RuleRHS
std::unique_ptr<Rule> ParseRule(const dt::InnerNode& rule) {
  const auto b = rule.ChildrenCbegin();
  auto [lhs, is_axiom] = ParseRuleLHS(static_cast<const dt::InnerNode&>(**b));
  auto rhs = ParseRuleRHS(static_cast<const dt::InnerNode&>(**(b + 2)));

  return std::make_unique<Rule>(std::move(lhs), std::move(rhs), is_axiom);
}

// Rules ::= Rule Rules | ε
std::vector<std::unique_ptr<Rule>> ParseRules(const dt::InnerNode& rules) {
  const auto b = rules.ChildrenCbegin();
  if (b == rules.ChildrenCend()) {
    return {};
  }

  auto rule = ParseRule(static_cast<const dt::InnerNode&>(**b));
  auto rules_rhs = ParseRules(static_cast<const dt::InnerNode&>(**(b + 1)));

  rules_rhs.push_back(std::move(rule));
  std::rotate(rules_rhs.rbegin(), rules_rhs.rbegin() + 1, rules_rhs.rend());
  return rules_rhs;
}

// Program ::= Rules
std::unique_ptr<Program> ParseProgram(const dt::InnerNode& program) {
  const auto b = program.ChildrenCbegin();
  auto rules = ParseRules(static_cast<const dt::InnerNode&>(**b));
  return std::make_unique<Program>(std::move(rules));
}

std::unique_ptr<Program> DtToAst(const dt::InnerNode& program) {
  return ParseProgram(program);
}

}  // namespace ast

}  // namespace parser
