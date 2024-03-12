#pragma once

#include "compiler.hpp"
#include "position.hpp"

namespace lexer {

class Compiler;

enum class DomainTag {
  kEndOfProgram,
  kIdent,
  kNumber,
  kString,
};

std::string_view ToString(const DomainTag tag) noexcept;

struct Token {
  DomainTag tag;
  Position starting;

  Token(const DomainTag tag, const Position& starting)
      : tag(tag), starting(starting) {}

  virtual ~Token() {}
};

struct IdentToken final : public Token {
  std::size_t code;

  IdentToken(const Position& starting, const std::size_t code)
      : Token(DomainTag::kIdent, starting), code(code) {}
};

struct NumberToken final : public Token {
  int value;

  NumberToken(const Position& starting, const int value)
      : Token(DomainTag::kNumber, starting), value(value) {}
};

struct StringToken final : public Token {
  std::string str;

  StringToken(const Position& starting, const std::string& str)
      : Token(DomainTag::kString, starting), str(str) {}
};

struct SpecToken final : public Token {
  using Token::Token;
};

void Print(std::ostream& os, const Token& token, const Compiler& compiler);

}  // namespace lexer
