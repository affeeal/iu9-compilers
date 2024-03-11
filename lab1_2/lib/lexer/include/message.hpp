#pragma once

#include <string>

namespace lexer {

enum class MessageType {
  kError,
  kWarning,
};

struct Message final {
  MessageType type;
  std::string text;

  Message(const MessageType type, const std::string& text)
      : type(type), text(text) {}
};

}  // namespace lexer
