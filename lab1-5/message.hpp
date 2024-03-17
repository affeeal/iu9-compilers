#pragma once

#include <string>

#include "position.hpp"

namespace lexer {

enum class MessageType {
  kError,
  kWarning,
};

std::ostream& operator<<(std::ostream& os, const MessageType type);

struct Message final {
  MessageType type;
  std::string text;
};

}  // namespace lexer
