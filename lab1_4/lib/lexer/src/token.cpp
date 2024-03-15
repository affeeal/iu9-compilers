#include "token.hpp"

namespace lexer {

std::string_view ToString(const DomainTag tag) noexcept {
  switch (tag) {
    case DomainTag::kComment: {
      return "COMMENT";
    }
                              
    case DomainTag::kEndOfProgram: {
      return "END_OF_PROGRAM";
    }

    case DomainTag::kExclamatory: {
      return "EXLAMATORY";
    }

    case DomainTag::kExclamatoryTilda: {
      return "EXLAMATORY_TILDA";
    }

    case DomainTag::kExist: {
      return "EXIST";
    }

    case DomainTag::kExit: {
      return "EXIT";
    }

    case DomainTag::kIdentifier: {
      return "IDENTIFIER";
    }

    case DomainTag::kNotFinal: {
      return "NOT_FINAL";
    }

    case DomainTag::kNumber: {
      return "NUMBER";
    }

    case DomainTag::kWhitespace: {
      return "WHITESPACE";
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Token* const token) {
  os << token->get_coords() << " " << ToString(token->get_tag()) << " ";

  switch (token->get_tag()) {
    using enum DomainTag;

    case kNumber: {
      const auto number = static_cast<const NumberToken* const>(token);
      os << number->get_value();
      break;
    }
  }

  return os;
}

}  // namespace lexer
