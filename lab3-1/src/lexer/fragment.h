#pragma once

#include "position.h"

namespace lexer {

struct Fragment final {
  Position starting;
  Position following;

  std::string ToString() const;
};

}  // namespace lexer
