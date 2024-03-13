#include "scanner.hpp"

#include <cstdint>
#include <regex>
#include <sstream>
#include <string>

#include "message.hpp"
#include "position.hpp"
#include "token.hpp"

namespace lexer {

namespace {

// error messages:
const std::string kOverflow = "integral constant is too large";
const std::string kNoOpeningQuote = "missing string opening double quote";
const std::string kNoClosingQuote = "missing string closing double quote";
const std::string kUnexpectedChar = "unexpected character";
const std::string kBreakLine = "line break is forbidden";
const std::string kBadEscape = "undefined escape sequence";

}  // namespace

std::unique_ptr<Token> Scanner::NextToken() {
  while (cur_.Cp() != kEnd) {
    while (cur_.IsWhitespace()) {
      cur_.Next();
    }

    const auto start = cur_;

    switch (cur_.Cp()) {
      case '0':
        return std::make_unique<NumberToken>(0, start, ++cur_);

      case '1': {
        static constexpr std::int64_t overflow_border = (INT64_MAX - 1) / 10;

        std::int64_t val = 0;

        do {
          if (val > overflow_border) {
            compiler_->AddMessage(MessageType::kError, start, kOverflow);

            auto token = std::make_unique<NumberToken>(val, start, cur_++);

            while (cur_.Cp() == '1') {
              cur_.Next();
            }

            return token;
          }

          val = val * 10 + 1;
        } while ((++cur_).Cp() == '1');

        return std::make_unique<NumberToken>(val, start, cur_);
      }

      case '\"': {
        while ((++cur_).Cp() != '\"' && !cur_.IsEnd()) {
          if (cur_.Cp() == '\\') {
            if ((++cur_).Cp() != '\"' && cur_.Cp() != 't' && cur_.Cp() != 'n') {
              compiler_->AddMessage(MessageType::kError, cur_, kBadEscape);
            }
          }

          if (cur_.IsNewLine()) {
            compiler_->AddMessage(MessageType::kError, cur_, kBreakLine);
          }
        }

        if (cur_.IsEnd()) {
          compiler_->AddMessage(MessageType::kError, start, kNoClosingQuote);
          return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
        }

        std::string str(program_->begin() + start.get_index() + 1,
                        program_->begin() + cur_.get_index());
        return std::make_unique<StringToken>(std::move(str), start, ++cur_);
      }

      case '@': {
        if ((++cur_).Cp() != '\"') {
          compiler_->AddMessage(MessageType::kError, start, kNoOpeningQuote);
          continue;
        }

        while (true) {
          while ((++cur_).Cp() != '\"' && !cur_.IsEnd())
            ;

          if (cur_.IsEnd()) {
            compiler_->AddMessage(MessageType::kError, start, kNoClosingQuote);
            return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
          }

          if ((++cur_).Cp() != '\"') {
            break;
          }
        }

        std::ostringstream t;
        std::ostreambuf_iterator<char> oi(t);

        auto begin = program_->begin() + start.get_index() + 2;
        auto end = program_->begin() + cur_.get_index() - 1;
        static const std::regex double_qoutes("\"\"");

        std::regex_replace(oi, begin, end, double_qoutes, "\"");
        return std::make_unique<StringToken>(t.str(), start, cur_);
      }

      case kEnd:
        return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);

      default: {
        compiler_->AddMessage(MessageType::kError, cur_++, kUnexpectedChar);
        break;
      }
    }
  }
}

}  // namespace lexer
