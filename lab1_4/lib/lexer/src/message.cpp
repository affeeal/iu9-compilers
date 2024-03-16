#include "message.hpp"

namespace lexer {

std::ostream& operator<<(std::ostream& os, const MessageType type) {
  switch (type) {
    case MessageType::kError: {
      os << "ERROR";
      break;
    }

    case MessageType::kWarning: {
      os << "WARNING";
      break;
    }
  }

  return os;
}

}  // namespace lexer
