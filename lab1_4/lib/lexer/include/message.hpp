#pragma once

#include <string>

#include "position.hpp"

namespace lexer {

const std::string kSyntaxError = "syntax error";

enum class MessageType {
  kError,
  kWarning,
};

std::ostream& operator<<(std::ostream& os, const MessageType type);

class Message final {
 public:
  Message() noexcept : type_(MessageType::kError) {}
  Message(const MessageType type, const std::string& text) noexcept
      : type_(type), text_(text) {}

  MessageType get_type() const noexcept { return type_; }
  const std::string& get_text() const& noexcept { return text_; }

 private:
  MessageType type_;
  std::string text_;
};

}  // namespace lexer
