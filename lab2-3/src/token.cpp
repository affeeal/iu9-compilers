#include "token.hpp"

#include <ostream>

namespace lexer {

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  switch (tag) {
    case DomainTag::kN: {
      os << "N";
      break;
    }

    case DomainTag::kPlus: {
      os << "PLUS";
      break;
    }

    case DomainTag::kStar: {
      os << "STAR";
      break;
    }

    case DomainTag::kLparen: {
      os << "LPAREN";
      break;
    }

    case DomainTag::kRparen: {
      os << "RPAREN";
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
  os << token.get_coords() << " " << token.get_tag();

  return os;
}

}  // namespace lexer
