#include "scanner.hpp"

#include <cstdint>
#include <regex>
#include <sstream>
#include <string>

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

        // case '\"': {
        //   // TODO
        // }

      case '@': {
        cur_.Next();

        if (cur_.Cp() != '\"') {
          compiler_->AddMessage(MessageType::kError, start,
                                "missing literal string opening double quote");
          break;
        }

        while (true) {
          do {
            cur_.Next();
          } while (cur_.Cp() != '\"' && cur_.Cp() != kEnd);

          if (cur_.Cp() == kEnd) {
            compiler_->AddMessage(
                MessageType::kError, start,
                "missing literal string closing double quote");
            return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_,
                                               cur_);
          }

          if ((cur_ + 1).Cp() != '\"') {
            break;
          }

          cur_ = cur_ + 2;
        }

        std::ostringstream t;
        std::ostreambuf_iterator<char> oi(t);

        auto begin = program_->begin() + start.get_index() + 2;
        auto end = program_->begin() + cur_.get_index();

        static const std::regex double_qoutes("\"\"");

        std::regex_replace(oi, begin, end, double_qoutes, "\"");
        return std::make_unique<StringToken>(t.str(), start, cur_++);
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
