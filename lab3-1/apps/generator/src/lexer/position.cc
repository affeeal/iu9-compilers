#include "position.h"

namespace lexer {

std::string Position::ToString() const {
  return '(' + std::to_string(line) + ", " + std::to_string(pos) + ')';
}

}  // namespace lexer
