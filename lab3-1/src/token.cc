#include "token.h"

namespace lexer {

std::string_view ToString(const DomainTag tag) {
  switch (tag) {
    case DomainTag::kNonTerminal: {
      return "NONTERMINAL";
    }
    case DomainTag::kTerminal: {
      return "TERMINAL";
    }
    case DomainTag::kOpArrow: {
      return "OP_ARROW";
    }
    case DomainTag::kKwAxiom: {
      return "KW_AXIOM";
    }
    case DomainTag::kKwEpsilon: {
      return "KW_EPSILON";
    }
    case DomainTag::kKwOr: {
      return "KW_OR";
    }
    case DomainTag::kKwEnd: {
      return "KW_END";
    }
    case DomainTag::kEndOfProgram: {
      return "END_OF_PROGRAM";
    }
  }
}

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  return os << ToString(tag);
}

}  // namespace lexer
