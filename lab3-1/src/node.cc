#include "node.h"

namespace parser {

std::string_view ToString(const NonTerminal non_terminal) {
  switch (non_terminal) {
    case NonTerminal::kProgram: {
      return "Program";
    }
    case NonTerminal::kRules: {
      return "Rules";
    }
    case NonTerminal::kRule: {
      return "Rule";
    }
    case NonTerminal::kRuleLHS: {
      return "RuleLHS";
    }
    case NonTerminal::kRuleRHS: {
      return "RuleRHS";
    }
    case NonTerminal::kExpr: {
      return "Expr";
    }
    case NonTerminal::kExpr1: {
      return "Expr1";
    }
    case NonTerminal::kTerm: {
      return "Term";
    }
    case NonTerminal::kTerm1: {
      return "Term1";
    }
    case NonTerminal::kSymbol: {
      return "Symbol";
    }
    case NonTerminal::kDummy: {
      return "Dummy";
    }
  }
}

std::ostream& operator<<(std::ostream& os, const NonTerminal nt) {
  return os << ToString(nt);
}

namespace dt {

INode& InnerNode::AddChild(std::unique_ptr<INode>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

}  // namespace dt

}  // namespace parser
