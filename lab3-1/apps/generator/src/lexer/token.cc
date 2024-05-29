#include "token.h"

#include <stdexcept>

namespace lexer {

void Token::ThrowError(const std::string& msg) const {
  throw std::runtime_error(coords_.ToString() + ": " + msg);
}

std::string ToString(const DomainTag tag) {
  switch (tag) {
    case DomainTag::kNonterminal: {
      return "NONTERMINAL";
    }
    case DomainTag::kTerminal: {
      return "TERMINAL";
    }
    case DomainTag::kArrow: {
      return "ARROW";
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
