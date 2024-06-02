#include "ast.h"

namespace fmt {

std::ostream& operator<<(std::ostream& os, const ElementaryType::Kind kind) {
  switch (kind) {
    case ElementaryType::Kind::kInt: {
      os << "int";
      break;
    }
  }

  return os;
}

}  // namespace fmt
