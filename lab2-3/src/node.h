#pragma once

namespace parser {

enum class RuleFor {
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

enum class Symbol {
};

struct Node {
  virtual ~Node() {}
};

struct Term final : Node {};

struct NTerm final : Node {};

}  // namespace parser
