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

}  // namespace lexer
