#pragma once

#include <string>

#include "position.hpp"

namespace lexer {

const std::string kSyntaxError = "syntax error";

enum class MessageType {
  kError,
  kOther,
  kWarning,
};

std::string_view ToString(const MessageType type) noexcept;

class Message final {
 public:
  Message() noexcept : type_(MessageType::kOther) {}
  Message(const MessageType type, const std::string& text) noexcept
      : type_(type), text_(text) {}

  MessageType get_type() const noexcept { return type_; }
  const std::string& get_text() const& noexcept { return text_; }

 private:
  MessageType type_;
  std::string text_;
};

void Print(std::ostream& os, const Message& message, const Position& position);

}  // namespace lexer
