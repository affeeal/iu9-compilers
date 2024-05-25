/*
#include "validator.h"

#include <algorithm>

#include "ast.h"

namespace parser {

namespace ast {

void Validator::Visit(const Program& program) {
  for (auto f = program.RulesCbegin(), l = program.RulesCend(); f != l; ++f) {
    const auto& rule = **f;
    if (rules.find(rule.get_name()) != rules.cend()) {
      throw std::runtime_error("Nonterminal redefinition: " + rule.get_name());
    }
    rules[rule.get_name()] = &rule;

    if (rule.get_is_axiom()) {
      if (axiom) {
        throw std::runtime_error("Axiom redefinition: " + rule.get_name());
      }
      axiom = &rule;
      involved_nonterminals_.insert(rule.get_name());
    }
  }

  if (!axiom) {
    throw std::runtime_error("Axiom is not defined");
  }

  for (auto f = program.RulesCbegin(), l = program.RulesCend(); f != l; ++f) {
    f->get()->Accept(*this);
  }

  const auto is_involved = [this](const std::unique_ptr<Rule>& rule) {
    return involved_nonterminals_.contains(rule->get_name());
  };

  if (const auto found = std::find_if_not(program.RulesCbegin(),
                                          program.RulesCend(), is_involved);
      found != program.RulesCend()) {
    throw std::runtime_error("Unused nonterminal " + found->get()->get_name());
  }
}

void Validator::Visit(const Rule& rule) {
  for (auto f = rule.TermsCbegin(), l = rule.TermsCend(); f != l; ++f) {
    f->get()->Accept(*this);
  }
}

void Validator::Visit(const Term& term) {
  for (auto f = term.SymsCbegin(), l = term.SymsCend(); f != l; ++f) {
    f->Accept(*this);
  }
}

void Validator::Visit(const Symbol& symbol) {
  if (symbol.get_type() != Symbol::Type::kNonterminal) {
    return;
  }

  if (rules.find(symbol.get_name()) == rules.cend()) {
    throw std::runtime_error("Undefined nonterminal " + symbol.get_name());
  }
  involved_nonterminals_.insert(symbol.get_name());
}

}  // namespace ast

}  // namespace parser
   // */
