#include "message.hpp"

namespace lexer {

std::string_view ToString(const MessageType type) noexcept {
  switch (type) {
    using enum MessageType;
    case kError:
      return "Error";
    case kWarning:
      return "Warning";
    case kOther:
      return "Other";
  }
}

}  // namespace lexer
