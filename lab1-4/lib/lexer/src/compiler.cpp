#include "compiler.hpp"

#include "message.hpp"

namespace lexer {

std::size_t Compiler::AddName(const std::string& name) {
  if (const auto it = name_codes_.find(name); it != name_codes_.cend()) {
    return it->second;
  }

  const auto code = names_.size();
  names_.push_back(name);
  name_codes_[name] = code;
  return code;
}

const std::string& Compiler::GetName(const std::size_t code) const& {
  return names_.at(code);
}

void Compiler::AddMessage(const MessageType type, const Position& p,
                          const std::string& text) {
  messages_[p] = Message(type, text);
}

void Compiler::OutputMessages(std::ostream& os) const {
  os << "MESSAGES:\n";

  for (const auto& [position, message] : messages_) {
    os << '\t' << message.get_type() << " " << position << ": "
       << message.get_text() << '\n';
  }
}

std::unique_ptr<Scanner> GetScanner(
    const std::shared_ptr<Compiler>& compiler,
    const std::shared_ptr<const std::string>& program) noexcept {
  return std::make_unique<Scanner>(program, compiler);
}

}  // namespace lexer
