#include "dt_to_ast.h"

#include <algorithm>
#include <iterator>

#include "symbol_table.h"

namespace parser {

namespace ast {

std::shared_ptr<const Program> DtToAst::Convert(const dt::InnerNode& program) {
  Reset();
  return ParseProgram(program);
}

void DtToAst::Reset() noexcept {
  symbol_table_ = SymbolTable{};
  axiom_ = nullptr;
}

// Program ::= Rules
std::unique_ptr<const Program> DtToAst::ParseProgram(
    const dt::InnerNode& program) {
  const auto& rules =
      static_cast<const dt::InnerNode&>(**program.ChildrenCbegin());
  auto ast_rules = ParseRules(rules);

  if (!axiom_) {
    throw std::runtime_error("axiom is not defined");
  }

  const auto symbol_table =
      std::make_shared<const SymbolTable>(std::move(symbol_table_));
  return std::make_unique<const Program>(std::move(ast_rules), symbol_table,
                                         axiom_);
}

// Rules ::= Rule Rules | ε
std::vector<std::unique_ptr<const Rule>> DtToAst::ParseRules(
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
std::unique_ptr<const Rule> DtToAst::ParseRule(const dt::InnerNode& rule) {
  const auto b = rule.ChildrenCbegin();

  const auto& rule_lhs = static_cast<const dt::InnerNode&>(**b);
  const auto* const lhs = ParseRuleLHS(rule_lhs);

  const auto& rule_rhs = static_cast<const dt::InnerNode&>(**(b + 2));
  auto rhs = ParseRuleRHS(rule_rhs);

  return std::make_unique<const Rule>(std::move(rhs), lhs);
}

// RuleLHS ::= KW_AXIOM NONTERMINAL | NONTERMINAL
const Nonterminal* DtToAst::ParseRuleLHS(const dt::InnerNode& rule_lhs) {
  const auto b = rule_lhs.ChildrenCbegin();
  if (rule_lhs.ChildrenCend() - b == 2) {
    const auto& leaf = static_cast<const dt::LeafNode&>(**(b + 1));
    const auto* const token =
        static_cast<const lexer::NonterminalToken*>(leaf.get_token());

    if (axiom_) {
      token->ThrowError("axiom redefinition");
    }

    const auto* const nonterminal =
        symbol_table_.GetNonterminal(token->get_str());
    axiom_ = nonterminal;
    return nonterminal;
  }

  const auto& leaf = static_cast<const dt::LeafNode&>(**b);
  const auto* const token =
      static_cast<const lexer::NonterminalToken*>(leaf.get_token());
  return symbol_table_.GetNonterminal(token->get_str());
}

// RuleRHS ::= Expr KW_END
std::vector<std::unique_ptr<const Term>> DtToAst::ParseRuleRHS(
    const dt::InnerNode& rule_rhs) {
  const auto& expr =
      static_cast<const dt::InnerNode&>(**rule_rhs.ChildrenCbegin());
  return ParseExpr(expr);
}

// Expr ::= Term Expr1
std::vector<std::unique_ptr<const Term>> DtToAst::ParseExpr(
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
std::vector<std::unique_ptr<const Term>> DtToAst::ParseExpr1(
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
std::unique_ptr<const Term> DtToAst::ParseTerm(const dt::InnerNode& term) {
  const auto b = term.ChildrenCbegin();
  if (term.ChildrenCend() - b == 1) {
    return std::make_unique<const Term>(std::vector<const ISymbol*>{});
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  const auto* const ast_symbol = ParseSymbol(symbol);

  const auto& term1 = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(term1);

  ast_term1.push_back(ast_symbol);
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return std::make_unique<const Term>(std::move(ast_term1));
}

// Term1 ::= Symbol Term1 | ε
std::vector<const ISymbol*> DtToAst::ParseTerm1(const dt::InnerNode& term1) {
  const auto b = term1.ChildrenCbegin();
  if (b == term1.ChildrenCend()) {
    return {};
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  const auto* const ast_symbol = ParseSymbol(symbol);

  const auto& term1_rhs = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(term1_rhs);

  ast_term1.push_back(ast_symbol);
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return ast_term1;
}

// Symbol ::= TERMINAL | NONTERMINAL
const ISymbol* DtToAst::ParseSymbol(const dt::InnerNode& symbol) {
  const auto& leaf =
      static_cast<const dt::LeafNode&>(**symbol.ChildrenCbegin());

  if (const auto* const terminal =
          dynamic_cast<const lexer::TerminalToken*>(leaf.get_token())) {
    return symbol_table_.GetTerminal(terminal->get_str());
  }

  const auto& nonterminal =
      static_cast<const lexer::NonterminalToken&>(*leaf.get_token());
  return symbol_table_.GetNonterminal(nonterminal.get_str());
}

}  // namespace ast

}  // namespace parser
