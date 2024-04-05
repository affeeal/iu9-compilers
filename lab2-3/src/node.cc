#include "node.h"

#include <algorithm>

namespace parser {

const auto kIndent = ".  ";

std::ostream& operator<<(std::ostream& os, const NonTerminal non_terminal) {
  switch (non_terminal) {
    case NonTerminal::kProgram: {
      os << "Program";
      break;
    }

    case NonTerminal::kRules: {
      os << "Rules";
      break;
    }

    case NonTerminal::kRule: {
      os << "Rule";
      break;
    }

    case NonTerminal::kRuleLHS: {
      os << "RuleLHS";
      break;
    }

    case NonTerminal::kRuleRHS: {
      os << "RuleRHS";
      break;
    }

    case NonTerminal::kExpr: {
      os << "Expr";
      break;
    }

    case NonTerminal::kExpr1: {
      os << "Expr1";
      break;
    }

    case NonTerminal::kTerm: {
      os << "Term";
      break;
    }

    case NonTerminal::kTerm1: {
      os << "Term1";
      break;
    }

    case NonTerminal::kFactor: {
      os << "Factor";
      break;
    }

    case NonTerminal::kDummy: {
      os << "Dummy";
      break;
    }
  }

  return os;
}

void InnerNode::Output(std::ostream& os, const std::string& indent) const {
  os << indent << non_terminal_ << " {\n";

  std::for_each(
      children_.cbegin(), children_.cend(),
      [&os, &indent](auto&& child) { child->Output(os, indent + kIndent); });

  os << indent << "}\n";
}

void LeafNode::Output(std::ostream& os, const std::string& indent) const {
  os << indent << token_->tag() << ": ";
  token_->OutputAttr(os);
  os << "\n";
}

}  // namespace parser
