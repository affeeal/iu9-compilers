#include "fragment.hpp"

#include <ostream>

namespace lexer {

void Fragment::Dump(std::ostream& os) const {
  os << starting_ << "-" << following_;
}

std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
  fragment.Dump(os);
  return os;
}

}  // namespace lexer
