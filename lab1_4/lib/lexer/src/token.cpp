#include "token.hpp"

namespace lexer {

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  switch (tag) {
    case DomainTag::kComment: {
      os << "COMMENT";
      break;
    }

    case DomainTag::kEndOfProgram: {
      os << "END_OF_PROGRAM";
      break;
    }

    case DomainTag::kExclamatory: {
      os << "EXLAMATORY";
      break;
    }

    case DomainTag::kExclamatoryTilda: {
      os << "EXLAMATORY_TILDA";
      break;
    }

    case DomainTag::kExist: {
      os << "EXIST";
      break;
    }

    case DomainTag::kExit: {
      os << "EXIT";
      break;
    }

    case DomainTag::kIdentifier: {
      os << "IDENTIFIER";
      break;
    }

    case DomainTag::kNotFinal: {
      os << "NOT_FINAL";
      break;
    }

    case DomainTag::kNumber: {
      os << "NUMBER";
      break;
    }

    case DomainTag::kWhitespace: {
      os << "WHITESPACE";
      break;
    }
  }

  return os;
}

void OutputToken(std::ostream& os, const Token* const token,
                 const Compiler& compiler) {
  os << token->get_coords() << " " << token->get_tag() << " ";

  switch (token->get_tag()) {
    case DomainTag::kNumber: {
      const auto number = static_cast<const NumberToken* const>(token);
      os << number->get_value();
      break;
    }

    case DomainTag::kIdentifier: {
      const auto ident = static_cast<const IdentToken* const>(token);
      os << compiler.GetName(ident->get_code());
      break;
    }
  }
}

}  // namespace lexer
