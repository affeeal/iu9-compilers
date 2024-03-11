#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "message.hpp"
#include "position.hpp"
#include "scanner.hpp"

namespace lexer {

class Scanner;

class Compiler final {
 public:
  static std::unique_ptr<Scanner> GetScanner(
      const std::shared_ptr<Compiler>& compiler,
      const std::shared_ptr<const std::string>& program) noexcept;

  std::size_t AddName(const std::string& name);
  const std::string& GetName(const std::size_t code) const&;

  void AddMessage(const MessageType type, const Position& p,
                  const std::string& text);
  void OutputMessages() const;

 private:
  std::map<Position, Message> messages_;
  std::unordered_map<std::string, std::size_t> name_codes_;
  std::vector<std::string> names_;
};

}  // namespace lexer
