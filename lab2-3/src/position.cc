#include "position.h"

#include <ostream>

namespace lexer {

void Position::Dump(std::ostream& os) const {
  os << '(' << line << ", " << pos << ')';
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
  position.Dump(os);
  return os;
}

}  // namespace lexer
