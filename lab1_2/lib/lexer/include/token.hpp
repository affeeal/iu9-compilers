#pragma once

#include "fragment.hpp"
#include "position.hpp"

namespace lexer {

enum class DomainTag {
  kIdent,
  kNumber,
  kString,
};

struct Token {
  DomainTag tag;
  Fragment coords;

 protected:
  Token(const DomainTag tag, const Position& starting,
        const Position& following)
      : tag(tag), coords(starting, following) {}

  virtual ~Token() {}
};

struct IdentToken final : public Token {
  std::size_t code;

  IdentToken(const Position& starting, const Position& following,
             const std::size_t code)
      : Token(DomainTag::kIdent, starting, following), code(code) {}
};

struct NumberToken final : public Token {
  std::int64_t value;

  NumberToken(const Position& starting, const Position& following,
              const std::int64_t value)
      : Token(DomainTag::kNumber, starting, following), value(value) {}
};

struct StringToken final : public Token {
  std::string str;

  StringToken(const Position& starting, const Position& following,
              const std::string& str)
      : Token(DomainTag::kString, starting, following), str(str) {}
};

}  // namespace lexer
