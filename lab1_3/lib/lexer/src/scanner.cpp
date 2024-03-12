#include "scanner.hpp"

#include <cstdint>

#include "message.hpp"
#include "position.hpp"
#include "token.hpp"

namespace lexer {

std::unique_ptr<Token> Scanner::NextToken() {
  while (cur_.Cp() != kEnd) {
    while (cur_.IsWhitespace()) {
      cur_.Next();
    }

    const auto start = cur_;

    switch (cur_.Cp()) {
      case '0':
        return std::make_unique<NumberToken>(0, start, cur_++);

      case '1': {
        static constexpr std::int64_t overflow_border = (INT64_MAX - 1) / 10;
        std::int64_t val = 0;

        do {
          if (val > overflow_border) {
            compiler_->AddMessage(MessageType::kError, start,
                                  "integral constant is too large");
            do {
              cur_.Next();
            } while (cur_.Cp() == '1');
            break;  // TODO? delimiter required
          }

          val = val * 10 + 1;
          cur_.Next();
        } while (cur_.Cp() == '1');

        return std::make_unique<NumberToken>(val, start, cur_);
      }

      case '@': {
        // TODO
      }

      case '\"': {
        // TODO
      }

      case kEnd:
        return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_,
                                           cur_);

      default: {
        compiler_->AddMessage(MessageType::kError, cur_++,
                              "unexpected character");
        break;
      }
    }
  }
}

}  // namespace lexer
