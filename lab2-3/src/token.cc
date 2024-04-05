#include "token.h"

#include <ostream>

namespace lexer {

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  switch (tag) {
    case DomainTag::kNonTerminal: {
      os << "NONTERMINAL";
      break;
    }

    case DomainTag::kTerminal: {
      os << "TERMINAL";
      break;
    }

    case DomainTag::kOpArrow: {
      os << "OP_ARROW";
      break;
    }

    case DomainTag::kKwAxiom: {
      os << "KW_AXIOM";
      break;
    }

    case DomainTag::kKwEpsilon: {
      os << "KW_EPSILON";
      break;
    }

    case DomainTag::kKwOr: {
      os << "KW_OR";
      break;
    }

    case DomainTag::kKwEnd: {
      os << "KW_END";
      break;
    }

    case DomainTag::kEndOfProgram: {
      os << "END_OF_PROGRAM";
      break;
    }
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << token.coords() << " " << token.tag() << ": ";
  token.OutputAttr(os);

  return os;
}

}  // namespace lexer
