#include "token.h"

#include <ostream>

namespace lexer {

std::string ToString(const DomainTag tag) {
  switch (tag) {
    case DomainTag::kPlus: {
      return "PLUS";
    }
    case DomainTag::kMinus: {
      return "MINUS";
    }
    case DomainTag::kStar: {
      return "STAR";
    }
    case DomainTag::kSlash: {
      return "SLASH";
    }
    case DomainTag::kEqual: {
      return "EQUAL";
    }
    case DomainTag::kComma: {
      return "COMMA";
    }
    case DomainTag::kColon: {
      return "COLON";
    }
    case DomainTag::kColonColon: {
      return "COLON_COLON";
    }
    case DomainTag::kSemicolon: {
      return "SEMICOLON";
    }
    case DomainTag::kParanthesisLeft: {
      return "PARENTHESIS_LEFT";
    }
    case DomainTag::kParanthesisRight: {
      return "PARENTHESIS_RIGHT";
    }
    case DomainTag::kCurlyBracketLeft: {
      return "CURLY_BRACKET_LEFT";
    }
    case DomainTag::kCurlyBracketRight: {
      return "CURLY_BRACKET_RIGHT";
    }
    case DomainTag::kSquareBracketLeft: {
      return "SQUARE_BRACKET_LEFT";
    }
    case DomainTag::kSquareBracketRight: {
      return "SQUARE_BRACKET_RIGHT";
    }
    case DomainTag::kInt: {
      return "INT";
    }
    case DomainTag::kIs: {
      return "IS";
    }
    case DomainTag::kEnd: {
      return "END";
    }
    case DomainTag::kIdent: {
      return "IDENT";
    }
    case DomainTag::kIntConst: {
      return "INT_CONST";
    }
    case DomainTag::kEndOfProgram: {
      return "END_OF_PROGRAM";
    }
  }
}

}  // namespace lexer
