#include "message.hpp"

namespace lexer {

std::ostream& operator<<(std::ostream& os, const MessageType type) {
  switch (type) {
    case MessageType::kError: {
      os << "Error";
      break;
    }

    case MessageType::kWarning: {
      os << "Warning";
      break;
    }
  }

  return os;
}

}  // namespace lexer
