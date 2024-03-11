#include "fragment.hpp"

namespace lexer {

Fragment::Fragment(const Position& starting, const Position& following)
    : starting(starting), following(following) {}

void Fragment::Dump(std::ostream& os) const {
  os << starting << "-" << following;
}

std::ostream& operator<<(std::ostream& os, const Fragment& f) {
  f.Dump(os);
  return os;
}

}  // namespace lexer
