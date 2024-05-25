#include "node.h"

namespace parser {

namespace dt {

std::string_view ToString(const Nonterminal nonterminal) {
  switch (nonterminal) {
    case Nonterminal::kProgram: {
      return "Program";
    }
    case Nonterminal::kRules: {
      return "Rules";
    }
    case Nonterminal::kRule: {
      return "Rule";
    }
    case Nonterminal::kRuleLHS: {
      return "RuleLHS";
    }
    case Nonterminal::kRuleRHS: {
      return "RuleRHS";
    }
    case Nonterminal::kExpr: {
      return "Expr";
    }
    case Nonterminal::kExpr1: {
      return "Expr1";
    }
    case Nonterminal::kTerm: {
      return "Term";
    }
    case Nonterminal::kTerm1: {
      return "Term1";
    }
    case Nonterminal::kSymbol: {
      return "Symbol";
    }
    case Nonterminal::kDummy: {
      return "Dummy";
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Nonterminal nt) {
  return os << ToString(nt);
}

INode& InnerNode::AddChild(std::unique_ptr<INode>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

}  // namespace dt

}  // namespace parser
