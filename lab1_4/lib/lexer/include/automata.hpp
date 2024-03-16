#pragma once

#include <array>

#include "position.hpp"
#include "token.hpp"

namespace lexer {

class Automata final {
 public:
  enum State {
    kComment1,
    kComment2,
    kEndOfProgram,
    kIdentifier,
    kKeyword1,
    kKeyword2,
    kKeyword3,
    kKeyword4,
    kKeyword5,
    kKeyword6,
    kNumber,
    kOperation1,
    kOperation2,
    kStart,
    kTrap,
    kWhitespace,
  };

  constexpr Automata() = default;

  State At(const State state, const unsigned char ch) const;

  bool IsFinal(const State state) const;

  DomainTag GetTag(const State state) const;

 private:
  static constexpr std::size_t kStates = 16;
  static constexpr std::size_t kCharacters = 129;
  static constexpr std::size_t kFactors = 13;

  enum Factor {
    kDigit,
    kEndCharacter,
    kExclamatory,
    kLetterE,
    kLetterI,
    kLetterS,
    kLetterT,
    kLetterX,
    kLineFeed,
    kOtherCharacter,
    kOtherLetter,
    kOtherWhitespace,
    kTilda,
  };

  static constexpr std::array<DomainTag, kStates> GetDomainTags() noexcept {
    return {
        DomainTag::kNotFinal,          // State::kComment1
        DomainTag::kComment,           // State::kComment2
        DomainTag::kEndOfProgram,      // State::kEndOfProgram
        DomainTag::kIdentifier,        // State::kIdentifier
        DomainTag::kIdentifier,        // State::kKeyword1
        DomainTag::kIdentifier,        // State::kKeyword2
        DomainTag::kIdentifier,        // State::kKeyword3
        DomainTag::kExit,              // State::kKeyword4
        DomainTag::kIdentifier,        // State::kKeyword5
        DomainTag::kExist,             // State::kKeyword6
        DomainTag::kNumber,            // State::kNumber
        DomainTag::kExclamatory,       // State::kOperation1
        DomainTag::kExclamatoryTilda,  // State::kOperation2
        DomainTag::kNotFinal,          // State::kStart
        DomainTag::kNotFinal,          // State::kTrap
        DomainTag::kWhitespace,        // State::kWhitespace
    };
  }

  static constexpr std::array<Factor, kCharacters> GetFactors() noexcept {
    std::array<Factor, kCharacters> factors;
    factors.fill(Factor::kOtherCharacter);

    for (std::size_t ch = 'a'; ch <= 'z'; ++ch) {
      factors[ch] = Factor::kOtherLetter;
    }

    for (std::size_t ch = 'A'; ch <= 'Z'; ++ch) {
      factors[ch] = Factor::kOtherLetter;
    }

    factors['e'] = Factor::kLetterE;
    factors['i'] = Factor::kLetterI;
    factors['s'] = Factor::kLetterS;
    factors['t'] = Factor::kLetterT;
    factors['x'] = Factor::kLetterX;

    for (std::size_t ch = '0'; ch <= '9'; ++ch) {
      factors[ch] = Factor::kDigit;
    }

    factors['!'] = Factor::kExclamatory;
    factors['~'] = Factor::kTilda;
    factors['\n'] = Factor::kLineFeed;

    for (const auto ch : {' ', '\t', '\r'}) {
      factors[ch] = Factor::kOtherWhitespace;
    }

    factors[Position::kEndCharacter] = Factor::kEndCharacter;

    return factors;
  }

  static constexpr std::array<std::array<State, kFactors>, kStates>
  GetTransitions() noexcept {
    std::array<std::array<State, kFactors>, kStates> table{};

    table[State::kStart] = {
        State::kNumber,        // Factor::kDigit
        State::kEndOfProgram,  // Factor::kEndCharacter
        State::kOperation1,    // Factor::kExclamatory
        State::kKeyword1,      // Factor::kLetterE
        State::kIdentifier,    // Factor::kLetterI
        State::kIdentifier,    // Factor::kLetterS
        State::kIdentifier,    // Factor::kLetterT
        State::kIdentifier,    // Factor::kLetterX
        State::kWhitespace,    // Factor::kLineFeed
        State::kTrap,          // Factor::kOtherCharacter
        State::kIdentifier,    // Factor::kOtherLetter
        State::kWhitespace,    // Factor::kOtherWhitespace
        State::kComment1,      // Factor::kTilda
    };

    table[State::kTrap].fill(State::kTrap);

    table[State::kWhitespace].fill(State::kTrap);
    table[State::kWhitespace][Factor::kLineFeed] = State::kWhitespace;
    table[State::kWhitespace][Factor::kOtherWhitespace] = State::kWhitespace;

    table[State::kIdentifier] = GetIdentifierTransitions();

    table[State::kNumber].fill(State::kTrap);
    table[State::kNumber][Factor::kDigit] = State::kNumber;

    table[State::kKeyword1] = GetIdentifierTransitions();
    table[State::kKeyword1][Factor::kLetterX] = State::kKeyword2;

    table[State::kKeyword2] = GetIdentifierTransitions();
    table[State::kKeyword2][Factor::kLetterI] = State::kKeyword3;

    table[State::kKeyword3] = GetIdentifierTransitions();
    table[State::kKeyword3][Factor::kLetterT] = State::kKeyword4;
    table[State::kKeyword3][Factor::kLetterS] = State::kKeyword5;

    table[State::kKeyword4] = GetIdentifierTransitions();

    table[State::kKeyword5] = GetIdentifierTransitions();
    table[State::kKeyword5][Factor::kLetterT] = State::kKeyword6;

    table[State::kKeyword6] = GetIdentifierTransitions();

    table[State::kOperation1].fill(State::kTrap);
    table[State::kOperation1][Factor::kTilda] = State::kOperation2;

    table[State::kOperation2].fill(State::kTrap);

    table[State::kComment1].fill(State::kComment1);
    table[State::kComment1][Factor::kLineFeed] = State::kComment2;

    table[State::kComment2].fill(State::kTrap);

    return table;
  }

  static constexpr std::array<State, kFactors>
  GetIdentifierTransitions() noexcept {
    return {
        State::kIdentifier,  // Factor::kDigit
        State::kTrap,        // Factor::kEndCharacter
        State::kTrap,        // Factor::kExclamatory
        State::kIdentifier,  // Factor::kLetterE
        State::kIdentifier,  // Factor::kLetterI
        State::kIdentifier,  // Factor::kLetterS
        State::kIdentifier,  // Factor::kLetterT
        State::kIdentifier,  // Factor::kLetterX
        State::kTrap,        // Factor::kLineFeed
        State::kTrap,        // Factor::kOtherCharacter
        State::kIdentifier,  // Factor::kOtherLetter
        State::kTrap,        // Factor::kOtherWhitespace
        State::kTrap,        // Factor::kTilda
    };
  }

  std::array<DomainTag, kStates> domain_tags_ = GetDomainTags();
  std::array<Factor, kCharacters> factors_ = GetFactors();
  std::array<std::array<State, kFactors>, kStates> transitions_ =
      GetTransitions();
};

}  // namespace lexer
