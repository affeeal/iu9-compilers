#pragma once

#include <string>

namespace lexer {

const std::string kSyntaxError = "syntax error";

enum class MessageType {
  kError,
  kOther,
  kWarning,
};

std::string_view ToString(const MessageType type) noexcept;

struct Message final {
  MessageType type;
  std::string text;

  Message() noexcept : type(MessageType::kOther) {}
  Message(const MessageType type, const std::string& text) noexcept
      : type(type), text(text) {}
};

}  // namespace lexer
