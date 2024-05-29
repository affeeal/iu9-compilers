#include "fragment.h"

namespace lexer {

std::string Fragment::ToString() const {
  return starting.ToString() + '-' + following.ToString();
}

std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
  return os << fragment.ToString();
}

}  // namespace lexer
