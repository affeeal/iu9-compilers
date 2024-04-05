#pragma once

#include <initializer_list>
#include <ostream>
#include <unordered_map>
#include <variant>
#include <vector>

#include "token.h"

namespace parser {

enum class NonTerminal {
  kProgram,
  kRules,
  kRule,
  kRuleLHS,
  kRuleRHS,
  kExpr,
  kExpr1,
  kTerm,
  kTerm1,
  kFactor,
};

class Node {
 public:
  virtual void Output(std::ostream& os, const std::string& indent) const = 0;

  virtual ~Node() {}
};

class InnerNode final : public Node {
 public:
  InnerNode(const NonTerminal nt) noexcept : nt_(nt) {}

  void AddChild(std::unique_ptr<Node>&& node) {
    children_.push_back(std::move(node));
  }

  void Output(std::ostream& os, const std::string& indent) const override;

 private:
  NonTerminal nt_;
  std::vector<std::unique_ptr<Node>> children_;
};

class LeafNode final : public Node {
 public:
  LeafNode(std::unique_ptr<lexer::Token>&& token) noexcept
      : token_(std::move(token)) {}

  void Output(std::ostream& os, const std::string& indent) const override;

 private:
  std::unique_ptr<lexer::Token> token_;
};

}  // namespace parser
