#pragma once

#include <iostream>
#include <memory>
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
  kSymbol,
  kDummy,
};

std::ostream& operator<<(std::ostream& os, const NonTerminal non_terminal);

class Node {
 public:
  virtual void Output(std::ostream& os = std::cout,
                      const std::string& indent = std::string()) const = 0;

  virtual ~Node() {}
};

class InnerNode final : public Node {
 public:
  InnerNode(const NonTerminal non_terminal) noexcept
      : non_terminal_(non_terminal) {}

  std::vector<std::unique_ptr<Node>>& Children() noexcept { return children_; }
  NonTerminal non_terminal() const noexcept { return non_terminal_; }

  Node& AddChild(std::unique_ptr<Node>&& node);

  void Output(std::ostream& os, const std::string& indent) const override;

 private:
  NonTerminal non_terminal_;
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
