#include "ast.h"

namespace fmt {

std::ostream& operator<<(std::ostream& os, const Op op) {
  switch (op) {
    case Op::kCons: {
      return os << ':';
    }
    case Op::kAdd: {
      return os << '+';
    }
    case Op::kSub: {
      return os << '-';
    }
    case Op::kMul: {
      return os << '*';
    }
    case Op::kDiv: {
      return os << '/';
    }
  }
}

std::ostream& operator<<(std::ostream& os, const ElementaryType::Kind kind) {
  switch (kind) {
    case ElementaryType::Kind::kInt: {
      return os << "int";
    }
  }
}

}  // namespace fmt
