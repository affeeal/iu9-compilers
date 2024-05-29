#pragma once

#include "scanner.h"

namespace parser {

class INode {
 public:
  virtual ~INode() = default;
};

class InnerNode final : public INode {
  std::vector<std::unique_ptr<INode>> children_;

 public:
  INode& AddChild(std::unique_ptr<INode>&& node);

  std::vector<std::unique_ptr<INode>>& Children() noexcept { return children_; }

  auto ChildrenCbegin() const noexcept { return children_.cbegin(); }
  auto ChildrenCend() const noexcept { return children_.cend(); }
};

class LeafNode final : public INode {
  std::unique_ptr<lexer::Token> token_;

 public:
  LeafNode(std::unique_ptr<lexer::Token>&& token) noexcept
      : token_(std::move(token)) {}

  const lexer::Token* get_token() const noexcept { return token_.get(); }
};

class Parser final {
 public:
  std::unique_ptr<INode> TopDownParse(lexer::IScanner& scanner);
};

}  // namespace parser
