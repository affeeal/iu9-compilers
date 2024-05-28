#include "fragment.h"

namespace lexer {

std::string Fragment::ToString() const {
  return starting.ToString() + '-' + following.ToString();
}

}  // namespace lexer
