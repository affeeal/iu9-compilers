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

class Token {
 public:
  DomainTag get_tag() const noexcept { return tag_; }
  const Position& get_starting() const& noexcept { return starting_; }

  virtual ~Token() {}

 protected:
  Token(const DomainTag tag, const Position& starting) noexcept
      : tag_(tag), starting_(starting) {}

  DomainTag tag_;
  Position starting_;
};

class IdentToken final : public Token {
 public:
  IdentToken(const std::size_t code, const Position& starting) noexcept
      : Token(DomainTag::kIdent, starting), code_(code) {}

  std::size_t get_code() const noexcept { return code_; }

 private:
  std::size_t code_;
};

class NumberToken final : public Token {
 public:
  NumberToken(const std::int64_t value, const Position& starting) noexcept
      : Token(DomainTag::kNumber, starting), value_(value) {}

  std::int64_t get_value() const noexcept { return value_; }

 private:
  std::int64_t value_;
};

class StringToken final : public Token {
 public:
  StringToken(const std::string& str, const Position& starting) noexcept
      : Token(DomainTag::kString, starting), str_(str) {}

  StringToken(std::string&& str, const Position& starting) noexcept
      : Token(DomainTag::kString, starting), str_(std::move(str)) {}

  const std::string& get_str() const& noexcept { return str_; }

 private:
  std::string str_;
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Position& starting) noexcept
      : Token(tag, starting) {}
};

void Print(std::ostream& os, const Token& token, const Compiler& compiler);

}  // namespace lexer
