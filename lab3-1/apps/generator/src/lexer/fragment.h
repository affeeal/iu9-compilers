#pragma once

#include "position.h"

namespace lexer {

struct Fragment final {
  Position starting;
  Position following;

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Fragment& fragment);

}  // namespace lexer
