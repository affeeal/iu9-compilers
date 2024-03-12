#include "token.hpp"

namespace lexer {

std::string_view ToString(const DomainTag tag) noexcept {
  switch (tag) {
    using enum DomainTag;

    case kEndOfProgram:
      return "END_OF_PROGRAM";

    case kNumber:
      return "NUMBER";

    case kString:
      return "STRING";
  }
}

void Print(std::ostream& os, const Token& token, const Compiler& compiler) {
  os << token.get_coords() << " " << ToString(token.get_tag()) << " ";

  switch (token.get_tag()) {
    using enum DomainTag;

    case kNumber: {
      const auto number = static_cast<const NumberToken* const>(&token);
      os << number->get_value();
      break;
    }

    case kString: {
      const auto str = static_cast<const StringToken* const>(&token);
      os << str->get_str();
      break;
    }
  }
}

}  // namespace lexer
