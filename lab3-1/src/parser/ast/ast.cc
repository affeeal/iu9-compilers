#include "ast.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <utility>
#include <vector>

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include "first_follow.h"
#include "symbol_table.h"

namespace parser {

namespace ast {

Program::Program(std::vector<std::unique_ptr<const Rule>>&& rules,
                 std::unique_ptr<const SymbolTable>&& symbol_table) noexcept
    : rules_(std::move(rules)), symbol_table_(std::move(symbol_table)) {
  assert(rules_.size() > 0);
}

// Symbol ::= TERMINAL | NONTERMINAL
const ISymbol* ParseSymbol(SymbolTable& symbol_table,
                           const dt::InnerNode& symbol) {
  const auto& leaf =
      static_cast<const dt::LeafNode&>(**symbol.ChildrenCbegin());

  if (const auto* const terminal =
          dynamic_cast<const lexer::TerminalToken*>(leaf.get_token())) {
    return symbol_table.GetTerminal(terminal->get_str());
  }

  const auto& nonterminal =
      static_cast<const lexer::NonterminalToken&>(*leaf.get_token());
  return symbol_table.GetNonterminal(nonterminal.get_str());
}

// Term1 ::= Symbol Term1 | ε
std::vector<const ISymbol*> ParseTerm1(SymbolTable& symbol_table,
                                       const dt::InnerNode& term1) {
  const auto b = term1.ChildrenCbegin();
  if (b == term1.ChildrenCend()) {
    return {};
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  const auto* const ast_symbol = ParseSymbol(symbol_table, symbol);

  const auto& term1_rhs = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(symbol_table, term1_rhs);

  ast_term1.push_back(ast_symbol);
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return ast_term1;
}

// Term ::= Symbol Term1 | KW_EPSILON
std::unique_ptr<const Term> ParseTerm(SymbolTable& symbol_table,
                                      const dt::InnerNode& term) {
  const auto b = term.ChildrenCbegin();
  if (term.ChildrenCend() - b == 1) {
    auto symbols = std::vector<const ISymbol*>{symbol_table.GetEpsilon()};
    return std::make_unique<const Term>(std::move(symbols));
  }

  const auto& symbol = static_cast<const dt::InnerNode&>(**b);
  const auto* const ast_symbol = ParseSymbol(symbol_table, symbol);

  const auto& term1 = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term1 = ParseTerm1(symbol_table, term1);

  ast_term1.push_back(ast_symbol);
  std::rotate(ast_term1.rbegin(), ast_term1.rbegin() + 1, ast_term1.rend());
  return std::make_unique<const Term>(std::move(ast_term1));
}

// Expr1 ::= KW_OR Term Expr1 | ε
std::vector<std::unique_ptr<const Term>> ParseExpr1(
    SymbolTable& symbol_table, const dt::InnerNode& expr1) {
  const auto b = expr1.ChildrenCbegin();
  if (b == expr1.ChildrenCend()) {
    return {};
  }

  const auto& term = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_term = ParseTerm(symbol_table, term);

  const auto& expr1_rhs = static_cast<const dt::InnerNode&>(**(b + 2));
  auto ast_expr1 = ParseExpr1(symbol_table, expr1_rhs);

  ast_expr1.push_back(std::move(ast_term));
  std::rotate(ast_expr1.rbegin(), ast_expr1.rbegin() + 1, ast_expr1.rend());
  return ast_expr1;
}

// Expr ::= Term Expr1
std::vector<std::unique_ptr<const Term>> ParseExpr(SymbolTable& symbol_table,
                                                   const dt::InnerNode& expr) {
  const auto b = expr.ChildrenCbegin();

  const auto& term = static_cast<const dt::InnerNode&>(**b);
  auto ast_term = ParseTerm(symbol_table, term);

  const auto& expr1 = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_expr1 = ParseExpr1(symbol_table, expr1);

  ast_expr1.push_back(std::move(ast_term));
  std::rotate(ast_expr1.rbegin(), ast_expr1.rbegin() + 1, ast_expr1.rend());
  return ast_expr1;
}

// RuleRHS ::= Expr KW_END
std::vector<std::unique_ptr<const Term>> ParseRuleRHS(
    SymbolTable& symbol_table, const dt::InnerNode& rule_rhs) {
  const auto& expr =
      static_cast<const dt::InnerNode&>(**rule_rhs.ChildrenCbegin());
  return ParseExpr(symbol_table, expr);
}

// RuleLHS ::= KW_AXIOM NONTERMINAL | NONTERMINAL
std::pair<const Nonterminal*, bool> ParseRuleLHS(
    SymbolTable& symbol_table, const dt::InnerNode& rule_lhs) {
  auto b = rule_lhs.ChildrenCbegin();
  auto is_axiom = false;
  if (rule_lhs.ChildrenCend() - b == 2) {
    b += 1;
    is_axiom = true;
  }

  const auto& leaf = static_cast<const dt::LeafNode&>(**b);
  const auto* const token =
      static_cast<const lexer::NonterminalToken*>(leaf.get_token());
  return {symbol_table.GetNonterminal(token->get_str()), is_axiom};
}

// Rule ::= RuleLHS ARROW RuleRHS
std::unique_ptr<const Rule> ParseRule(SymbolTable& symbol_table,
                                      const dt::InnerNode& rule) {
  const auto b = rule.ChildrenCbegin();

  const auto& rule_lhs = static_cast<const dt::InnerNode&>(**b);
  const auto [lhs, is_axiom] = ParseRuleLHS(symbol_table, rule_lhs);

  const auto& rule_rhs = static_cast<const dt::InnerNode&>(**(b + 2));
  auto rhs = ParseRuleRHS(symbol_table, rule_rhs);

  return std::make_unique<const Rule>(std::move(rhs), lhs, is_axiom);
}

// Rules ::= Rule Rules | ε
std::vector<std::unique_ptr<const Rule>> ParseRules(
    SymbolTable& symbol_table, const dt::InnerNode& rules) {
  const auto b = rules.ChildrenCbegin();
  if (b == rules.ChildrenCend()) {
    return {};  // TODO: fix to contain at least one rule
  }

  const auto& rule = static_cast<const dt::InnerNode&>(**b);
  auto ast_rule = ParseRule(symbol_table, rule);

  const auto& rules_rhs = static_cast<const dt::InnerNode&>(**(b + 1));
  auto ast_rules = ParseRules(symbol_table, rules_rhs);

  ast_rules.push_back(std::move(ast_rule));
  std::rotate(ast_rules.rbegin(), ast_rules.rbegin() + 1, ast_rules.rend());
  return ast_rules;
}

// Program ::= Rules
std::unique_ptr<const Program> ParseProgram(const dt::InnerNode& program) {
  auto symbol_table = SymbolTable{};

  const auto& rules =
      static_cast<const dt::InnerNode&>(**program.ChildrenCbegin());
  auto ast_rules = ParseRules(symbol_table, rules);

  return std::make_unique<const Program>(
      std::move(ast_rules),
      std::make_unique<const SymbolTable>(std::move(symbol_table)));
}

std::shared_ptr<const Program> DtToAst(const dt::InnerNode& program) {
  return ParseProgram(program);
}

void Validate(const Program& program) {
  const Rule* axiom = nullptr;
  std::unordered_set<const Nonterminal*> defined_nonterminals,
      involved_nonterminals;

  for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
    const auto& rule = **b;
    if (const auto [_, is_inserted] =
            defined_nonterminals.insert(rule.get_lhs());
        !is_inserted) {
      throw std::runtime_error("Nonterminal redefinition: " +
                               rule.get_lhs()->get_name());
    }

    if (rule.get_is_axiom()) {
      if (axiom) {
        throw std::runtime_error("Axiom redefinition: " +
                                 rule.get_lhs()->get_name());
      }
      axiom = &rule;
      involved_nonterminals.insert(rule.get_lhs());
    }
  }

  if (!axiom) {
    throw std::runtime_error("Axiom is not defined");
  }

  for (auto b = program.RulesCbegin(), e = program.RulesCend(); b != e; ++b) {
    const auto& rule = **b;

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;

      for (auto b = term.SymsCbegin(), e = term.SymsCend(); b != e; ++b) {
        const auto* const nonterminal = dynamic_cast<const Nonterminal*>(*b);
        if (!nonterminal) {
          continue;
        }

        if (!defined_nonterminals.contains(nonterminal)) {
          throw std::runtime_error("Undefined nonterminal " +
                                   nonterminal->get_name());
        }
        involved_nonterminals.insert(nonterminal);
      }
    }
  }

  const auto is_involved =
      [&involved_nonterminals](const std::unique_ptr<const Rule>& rule) {
        return involved_nonterminals.contains(rule->get_lhs());
      };

  if (const auto it = std::find_if_not(program.RulesCbegin(),
                                       program.RulesCend(), is_involved);
      it != program.RulesCend()) {
    throw std::runtime_error("Unused nonterminal " +
                             it->get()->get_lhs()->get_name());
  }
}

std::unordered_map<TableKey, std::vector<const ISymbol*>> BuildTable(
    const FirstFollow& first_follow) {
  // TODO
}

}  // namespace ast

}  // namespace parser
