#include "message.h"

namespace lexer {

std::ostream& operator<<(std::ostream& os, const MessageType type) {
  switch (type) {
    case MessageType::kError: {
      os << "Error";
      break;
    }
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const Message& message) {
  os << message.type << " " << message.text;
  return os;
}

}  // namespace lexer
