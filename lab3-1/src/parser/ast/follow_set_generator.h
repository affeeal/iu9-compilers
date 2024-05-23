#pragma once

#include "ast.h"
#include "validator.h"

namespace parser {

namespace ast {

class FollowSetGenerator final : public IVisitor {
  std::unordered_map<std::string, std::unordered_set<TableSymbol>> follow_sets_;
  std::unordered_set<TableSymbol> return_;
  const Index& index_;

 public:
  FollowSetGenerator(const Index& index) noexcept : index_(index) {}

  void Visit(const Program& program) override;
  void Visit(const Rule& rule) override;
  void Visit(const Term& term) override;
  void Visit(const Symbol& symbol) override;
};

}  // namespace ast

}  // namespace parser
