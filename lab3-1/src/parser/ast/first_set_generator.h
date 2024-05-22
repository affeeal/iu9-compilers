#pragma once

#include "validator.h"
#include "visitor.h"

namespace parser {

namespace ast {

class FirstSetGenerator final : public IVisitor {
  const Index& index_;
  bool sets_are_changing_;
  std::unordered_map<std::string, std::unordered_set<std::string>> first_sets_;
  std::unordered_set<std::string> return_;

 public:
  FirstSetGenerator(const Index& index) noexcept : index_(index) {}

  void Visit(const Program& program) override;
  void Visit(const Rule& rule) override;
  void Visit(const Term& term) override;
  void Visit(const Symbol& symbol) override;
};

}  // namespace ast

}  // namespace parser
