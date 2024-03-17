#include "message.hpp"

namespace lexer {

std::string_view ToString(const MessageType type) noexcept {
  switch (type) {
    using enum MessageType;

    case kError:
      return "Error";

    case kOther:
      return "Other";

    case kWarning:
      return "Warning";
  }
}

void Print(std::ostream& os, const Message& message, const Position& position) {
  os << ToString(message.get_type()) << " " << position << ": "
     << message.get_text();
}

}  // namespace lexer
