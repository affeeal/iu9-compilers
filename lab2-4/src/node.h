#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "token.h"

namespace parser {

enum class NonTerminal {
  kProgram,
  kFunc,
  kFuncType,
  kType,
  kListType,
  kTupleType,
  kFuncBody,
  kStatement,
  kPattern,
  kPatternUnit,
  kPatternList,
  kPatternTuple,
  kResult,
  kResultUnit,
  kResultList,
  kResultTuple,
  kExpr,
  kTerm,
  kFactor,
  kAtom,
  kFuncArg,
};

std::ostream& operator<<(std::ostream& os, const NonTerminal non_terminal);

class Node {
 public:
  virtual ~Node() = default;
};

class InnerNode final : public Node {
 public:
  InnerNode(const NonTerminal non_terminal) noexcept
      : non_terminal_(non_terminal) {}

  NonTerminal get_non_terminal() const noexcept { return non_terminal_; }

  Node& AddChild(std::unique_ptr<Node>&& node);

 private:
  NonTerminal non_terminal_;
  std::vector<std::unique_ptr<Node>> children_;
};

class LeafNode final : public Node {
 public:
  LeafNode(std::unique_ptr<lexer::Token>&& token) noexcept
      : token_(std::move(token)) {}

 private:
  std::unique_ptr<lexer::Token> token_;
};

}  // namespace parser
