#include "token.hpp"

#include <ostream>

namespace lexer {

std::ostream& operator<<(std::ostream& os, const DomainTag tag) {
  switch (tag) {
    case DomainTag::kArrow: {
      os << "ARROW";
      break;
    }

    case DomainTag::kEndOfProgram: {
      os << "END_OF_PROGRAM";
      break;
    }

    case DomainTag::kCoefficient: {
      os << "COEFFICIENT";
      break;
    }

    case DomainTag::kPlus: {
      os << "PLUS";
      break;
    }

    case DomainTag::kSubstance: {
      os << "SUBSTANCE";
      break;
    }
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
  const auto tag = token.get_tag();
  const auto& coords = token.get_coords();

  os << coords << " " << tag;

  if (tag == DomainTag::kCoefficient) {
    const auto& coeff = static_cast<const CoefficientToken&>(token);
    os << ' ' << coeff.get_value();

  } else if (tag == DomainTag::kSubstance) {
    const auto& substance = static_cast<const SubstanceToken&>(token);
    os << ' ' << substance.get_str();
  }

  return os;
}

}  // namespace lexer
