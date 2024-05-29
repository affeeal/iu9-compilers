#pragma once

#include <cstdint>

#include "fragment.h"

namespace lexer {

enum class DomainTag {
  kNumber,
  kPlus,
  kStar,
  kLeftParenthesis,
  kRightParenthesis,
  kEndOfProgram,
};

std::string ToString(const DomainTag tag);
std::ostream& operator<<(std::ostream& os, const DomainTag tag);

class Token {
 public:
  virtual ~Token() = default;

  DomainTag get_tag() const noexcept { return tag_; }
  const Fragment& get_coords() const noexcept { return coords_; }

  [[noreturn]] void ThrowError(const std::string& msg) const;

 protected:
  Token(const DomainTag tag, const Fragment& coords) noexcept
      : tag_(tag), coords_(coords) {}

  DomainTag tag_;
  Fragment coords_;
};

class NumberToken final : public Token {
  std::uint64_t value_;

 public:
  NumberToken(const std::uint64_t value, const Fragment& coords) noexcept
      : Token(DomainTag::kNumber, coords), value_(value) {}

  std::uint64_t get_value() const noexcept { return value_; }
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

}  // namespace lexer
