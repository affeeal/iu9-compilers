#pragma once

#include "compiler.hpp"
#include "fragment.hpp"
#include "position.hpp"

namespace lexer {

class Compiler;

enum class DomainTag {
  kEndOfProgram,
  kNumber,
  kString,
};

std::string_view ToString(const DomainTag tag) noexcept;

class Token {
 public:
  DomainTag get_tag() const noexcept { return tag_; }
  const Fragment& get_coords() const& noexcept { return coords_; }

  virtual ~Token() {}

 protected:
  Token(const DomainTag tag, const Position& starting,
        const Position& following) noexcept
      : tag_(tag), coords_(starting, following) {}

  DomainTag tag_;
  Fragment coords_;
};

class NumberToken final : public Token {
 public:
  NumberToken(const std::int64_t value, const Position& starting,
              const Position& following) noexcept
      : Token(DomainTag::kNumber, starting, following), value_(value) {}

  std::int64_t get_value() const noexcept { return value_; }

 private:
  std::int64_t value_;
};

class StringToken final : public Token {
 public:
  StringToken(const std::string& str, const Position& starting,
              const Position& following) noexcept
      : Token(DomainTag::kString, starting, following), str_(str) {}

  StringToken(std::string&& str, const Position& starting,
              const Position& following) noexcept
      : Token(DomainTag::kString, starting, following), str_(std::move(str)) {}

  const std::string& get_str() const& noexcept { return str_; }

 private:
  std::string str_;
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Position& starting,
            const Position& following) noexcept
      : Token(tag, starting, following) {}

  SpecToken(const DomainTag tag, const Position& starting) noexcept
      : Token(tag, starting, starting) {}
};

std::ostream& operator<<(std::ostream& os, const Token* const token);

}  // namespace lexer
