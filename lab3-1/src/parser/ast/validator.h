#pragma once
/*

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast.h"
#include "visitor.h"

namespace parser {

namespace ast {

// TODO: rewrite as a function

class Validator final : public IVisitor {
  const Rule* axiom = nullptr;
  std::unordered_map<std::string, const Rule*> rules;
  std::unordered_set<std::string> involved_nonterminals_;

 public:
  void Visit(const Program& program) override;
  void Visit(const Rule& rule) override;
  void Visit(const Term& term) override;
  void Visit(const Symbol& symbol) override;
};

}  // namespace ast

}  // namespace parser
   // */
