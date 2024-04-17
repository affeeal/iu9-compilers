#include "token.h"

#include <ostream>

namespace lexer {

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  switch (tag) {
    case DomainTag::kPlus: {
      os << "PLUS";
      break;
    }

    case DomainTag::kMinus: {
      os << "MINUS";
      break;
    }

    case DomainTag::kStar: {
      os << "STAR";
      break;
    }

    case DomainTag::kSlash: {
      os << "SLASH";
      break;
    }

    case DomainTag::kEqual: {
      os << "EQUAL";
      break;
    }

    case DomainTag::kComma: {
      os << "COMMA";
      break;
    }

    case DomainTag::kColon: {
      os << "COLON";
      break;
    }

    case DomainTag::kColonColon: {
      os << "COLON_COLON";
      break;
    }

    case DomainTag::kSemicolon: {
      os << "SEMICOLON";
      break;
    }

    case DomainTag::kParanthesisLeft: {
      os << "PARENTHESIS_LEFT";
      break;
    }

    case DomainTag::kParanthesisRight: {
      os << "PARENTHESIS_RIGHT";
      break;
    }

    case DomainTag::kCurlyBracketLeft: {
      os << "CURLY_BRACKET_LEFT";
      break;
    }

    case DomainTag::kCurlyBracketRight: {
      os << "CURLY_BRACKET_RIGHT";
      break;
    }

    case DomainTag::kSquareBracketLeft: {
      os << "SQUARE_BRACKET_LEFT";
      break;
    }

    case DomainTag::kSquareBracketRight: {
      os << "SQUARE_BRACKET_RIGHT";
      break;
    }

    case DomainTag::kInt: {
      os << "INT";
      break;
    }

    case DomainTag::kIs: {
      os << "IS";
      break;
    }

    case DomainTag::kEnd: {
      os << "END";
      break;
    }

    case DomainTag::kIdent: {
      os << "IDENT";
      break;
    }

    case DomainTag::kIntConst: {
      os << "INT_CONST";
      break;
    }

    case DomainTag::kEndOfProgram: {
      os << "END_OF_PROGRAM";
      break;
    }
  }

  return os;
}

}  // namespace lexer
