#pragma once

#include <string>

#include "position.h"

namespace lexer {

enum class MessageType {
  kError,
};

std::ostream& operator<<(std::ostream& os, const MessageType type);

struct Message final {
  MessageType type;
  std::string text;
};

std::ostream& operator<<(std::ostream& os, const Message& message);

}  // namespace lexer
