#pragma once

#include <string>
#include <unordered_set>

#include "ast.h"
#include "visitor.h"

namespace parser {

namespace ast {

class Validator final : public IVisitor {
  Index index_;
  std::unordered_set<std::string> involved_nonterminals_;

 public:
  const Index& get_index() const noexcept { return index_; }

  void Visit(const Program& program) override;
  void Visit(const Rule& rule) override;
  void Visit(const Term& term) override;
  void Visit(const Symbol& symbol) override;
};

}  // namespace ast

}  // namespace parser
