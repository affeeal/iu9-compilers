#include "scanner.hpp"

#include <sstream>

#include "automata.hpp"

namespace lexer {

static constexpr Automata automata{};

std::unique_ptr<Token> Scanner::NextToken() {
  std::ostringstream image;
  Position start;
  Automata::State state, last_final;
  DomainTag tag;

  while (true) {
    if (cur_.IsEnd()) {
      return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
    }

    image.str(std::string());
    start = cur_;
    state = Automata::State::kStart;
    last_final = state;

    while (true) {
      state = automata.At(state, cur_.Cp());

      if (state == Automata::State::kTrap) {
        tag = automata.GetTag(last_final);

        if (tag == DomainTag::kWhitespace) {
          break;

        } else if (tag == DomainTag::kNotFinal) {
          compiler_->AddMessage(MessageType::kError, cur_++, kSyntaxError);
          break;

        } else if (tag == DomainTag::kComment) {
          comments_.emplace_back(start, cur_);
          break;

        } else if (tag == DomainTag::kExclamatory ||
                   tag == DomainTag::kExclamatoryTilda ||
                   tag == DomainTag::kExist || tag == DomainTag::kExit) {
          return std::make_unique<SpecToken>(tag, start, cur_);

        } else if (tag == DomainTag::kIdentifier) {
          const auto code = compiler_->AddName(image.str());
          return std::make_unique<IdentToken>(code, start, cur_);

        } else if (tag == DomainTag::kNumber) {
          const auto value = std::stoll(image.str());
          return std::make_unique<NumberToken>(value, start, cur_);
        }
      }

      if (automata.IsFinal(state)) {
        last_final = state;
      }

      image << (cur_++).Cp();
    }
  }
}

}  // namespace lexer
