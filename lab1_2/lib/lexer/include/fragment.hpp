#pragma once

#include "position.hpp"
namespace lexer {

struct Fragment final {
  Position starting, following;

  Fragment(const Position& starting, const Position& following);

  void Dump(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Fragment& f);

}  // namespace lexer
