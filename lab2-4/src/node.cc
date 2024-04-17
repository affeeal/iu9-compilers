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

    case NonTerminal::kFunc: {
      os << "Func";
      break;
    }

    case NonTerminal::kFuncType: {
      os << "FuncType";
      break;
    }

    case NonTerminal::kType: {
      os << "Type";
      break;
    }

    case NonTerminal::kListType: {
      os << "ListType";
      break;
    }

    case NonTerminal::kTupleType: {
      os << "TupleType";
      break;
    }

    case NonTerminal::kFuncBody: {
      os << "FuncBody";
      break;
    }

    case NonTerminal::kStatement: {
      os << "Statement";
      break;
    }

    case NonTerminal::kPattern: {
      os << "Pattern";
      break;
    }

    case NonTerminal::kPatternUnit: {
      os << "PatternUnit";
      break;
    }

    case NonTerminal::kPatternList: {
      os << "PatternList";
      break;
    }

    case NonTerminal::kPatternTuple: {
      os << "PatternTuple";
      break;
    }

    case NonTerminal::kResult: {
      os << "Result";
      break;
    }

    case NonTerminal::kResultUnit: {
      os << "ResultUnit";
      break;
    }

    case NonTerminal::kResultList: {
      os << "ResultList";
      break;
    }

    case NonTerminal::kResultTuple: {
      os << "ResultTuple";
      break;
    }

    case NonTerminal::kExpr: {
      os << "Expr";
      break;
    }

    case NonTerminal::kTerm: {
      os << "Term";
      break;
    }

    case NonTerminal::kFactor: {
      os << "Factor";
      break;
    }

    case NonTerminal::kAtom: {
      os << "Atom";
      break;
    }

    case NonTerminal::kFuncArg: {
      os << "FuncArg";
      break;
    }
  }

  return os;
}

Node& InnerNode::AddChild(std::unique_ptr<Node>&& node) {
  children_.push_back(std::move(node));
  return *children_.back();
}

}  // namespace parser
