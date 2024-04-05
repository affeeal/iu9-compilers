#pragma once

#include <map>

#include "message.h"

namespace lexer {

class Compiler final {
 public:
  auto MessagesCbegin() const& noexcept { return messages_.cbegin(); }
  auto MessagesCend() const& noexcept { return messages_.cend(); }

  void AddMessage(const MessageType type, const Position& p,
                  const std::string& text);

 private:
  std::map<Position, Message> messages_;
};

}  // namespace lexer
