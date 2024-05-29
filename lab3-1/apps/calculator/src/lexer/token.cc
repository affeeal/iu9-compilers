#include "token.h"

#include <stdexcept>

namespace lexer {

void Token::ThrowError(const std::string& msg) const {
  throw std::runtime_error(coords_.ToString() + ": " + msg);
}

std::string ToString(const DomainTag tag) {
  switch (tag) {
    case DomainTag::kNumber: {
      return "n";
    }
    case DomainTag::kPlus: {
      return "+";
    }
    case DomainTag::kStar: {
      return "*";
    }
    case DomainTag::kLeftParenthesis: {
      return "(";
    }
    case DomainTag::kRightParenthesis: {
      return ")";
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
