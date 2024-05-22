#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "visitor.h"

namespace parser {

namespace ast {

struct Index final {
  const Rule* axiom = nullptr;
  std::unordered_map<std::string, const Rule*> rules;
};

class Validator final : public IVisitor {
  Index index_;
  std::unordered_set<std::string> involved_nonterminals_;

 public:
  void Visit(const Program& program) override;
  void Visit(const Rule& rule) override;
  void Visit(const Term& term) override;
  void Visit(const Symbol& symbol) override;
};

}  // namespace ast

}  // namespace parser
