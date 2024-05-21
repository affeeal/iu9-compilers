#pragma once

#include <memory>
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
  kSymbol,
  kDummy,
};

std::string_view ToString(const NonTerminal nt);
std::ostream& operator<<(std::ostream& os, const NonTerminal nt);

namespace dt {

using Symbol = std::variant<NonTerminal, lexer::DomainTag>;

class INode {
 public:
  virtual ~INode() = default;
};

class InnerNode final : public INode {
  NonTerminal nt_;
  std::vector<std::unique_ptr<INode>> children_;

 public:
  InnerNode(const NonTerminal non_terminal) noexcept : nt_(non_terminal) {}

  std::vector<std::unique_ptr<INode>>& Children() noexcept { return children_; }

  auto ChildrenCbegin() const noexcept { return children_.cbegin(); }
  auto ChildrenCend() const noexcept { return children_.cend(); }

  NonTerminal get_nt() const noexcept { return nt_; }

  INode& AddChild(std::unique_ptr<INode>&& node);
};

class LeafNode final : public INode {
  std::unique_ptr<lexer::Token> token_;

 public:
  LeafNode(std::unique_ptr<lexer::Token>&& token) noexcept
      : token_(std::move(token)) {}

  const lexer::Token* get_token() const noexcept { return token_.get(); }
};

}  // namespace dt

}  // namespace parser
