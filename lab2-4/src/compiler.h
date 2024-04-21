#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "message.h"
#include "position.h"

namespace lexer {

class Compiler final {
 public:
  auto MessagesCbegin() const& noexcept { return messages_.cbegin(); }
  auto MessagesCend() const& noexcept { return messages_.cend(); }

  std::size_t AddName(const std::string& name);
  const std::string& GetName(const std::size_t code) const&;

  void AddMessage(const MessageType type, const Position& p,
                  const std::string& text);

 private:
  std::map<Position, Message> messages_;
  std::unordered_map<std::string, std::size_t> name_codes_;
  std::vector<std::string> names_;
};

}  // namespace lexer
