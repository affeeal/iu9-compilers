#include "token.hpp"

namespace lexer {

std::string_view ToString(const DomainTag tag) noexcept {
  switch (tag) {
    using enum DomainTag;
    
    case kEndOfProgram:
      return "END_OF_PROGRAM";

    case kIdent:
      return "IDENT";

    case kNumber:
      return "NUMBER";

    case kString:
      return "STRING";
  }
}

void Print(std::ostream& os, const Token& token,
           const Compiler& compiler) {
  os << ToString(token.tag) << " " << token.starting << ": ";

  switch (token.tag) {
    using enum DomainTag;

    case kIdent: {
      const auto ident = static_cast<const IdentToken* const>(&token);
      const auto& name = compiler.GetName(ident->code);  // TODO: check
      os << name;
      break;
    }

    case kNumber: {
      const auto number = static_cast<const NumberToken* const>(&token);
      os << number->value;
      break;
    }

    case kString: {
      const auto str = static_cast<const StringToken* const>(&token);
      os << str->str;
      break;
    }
  }
}

}  // namespace lexer
