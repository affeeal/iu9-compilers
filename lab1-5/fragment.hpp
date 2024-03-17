#pragma once

#include "position.hpp"

namespace lexer {

struct Fragment final {
  Position starting;
  Position following;

  void Dump(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Fragment& fragment);

}  // namespace lexer
