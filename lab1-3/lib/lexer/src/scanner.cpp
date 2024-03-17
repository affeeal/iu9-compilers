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
      case '0': {
        return std::make_unique<NumberToken>(0, start, ++cur_);
      }

      case '1': {
        std::int64_t val = 1;

        while ((++cur_).Cp() == '1') {
          ++val;
        }

        return std::make_unique<NumberToken>(val, start, cur_);
      }

      case '\"': {
        std::ostringstream oss;

        while ((++cur_).Cp() != '\"' && !cur_.IsEnd()) {
          if (cur_.Cp() == '\\') {
            switch ((++cur_).Cp()) {
              case '\"': {
                oss << '\"';
                break;
              }

              case 't': {
                oss << '\t';
                break;
              }

              case 'n': {
                oss << '\n';
                break;
              }

              default: {
                compiler_->AddMessage(MessageType::kError, cur_, kBadEscape);
              }
            }
          } else if (cur_.IsNewLine()) {
            compiler_->AddMessage(MessageType::kError, cur_, kBreakLine);
          } else {
            oss << cur_.Cp();
          }
        }

        if (cur_.IsEnd()) {
          compiler_->AddMessage(MessageType::kError, start, kNoClosingQuote);
          return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
        }

        return std::make_unique<StringToken>(oss.str(), start, ++cur_);
      }

      case '@': {
        std::ostringstream oss;

        if ((++cur_).Cp() != '\"') {
          compiler_->AddMessage(MessageType::kError, start, kNoOpeningQuote);
          continue;
        }

        while (true) {
          while ((++cur_).Cp() != '\"' && !cur_.IsEnd()) {
            oss << cur_.Cp();
          }

          if (cur_.IsEnd()) {
            compiler_->AddMessage(MessageType::kError, start, kNoClosingQuote);
            return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
          }

          if ((++cur_).Cp() == '\"') {
            oss << '\"';
          } else {
            break;
          }
        }

        return std::make_unique<StringToken>(oss.str(), start, cur_);
      }

      case kEnd: {
        return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
      }

      default: {
        compiler_->AddMessage(MessageType::kError, cur_++, kUnexpectedChar);
      }
    }
  }
}

}  // namespace lexer
