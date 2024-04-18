#pragma once

#include "fragment.h"

namespace lexer {

enum class DomainTag {
  kPlus,
  kMinus,
  kStar,
  kSlash,
  kEqual,
  kComma,
  kColon,
  kColonColon,
  kSemicolon,
  kParanthesisLeft,
  kParanthesisRight,
  kCurlyBracketLeft,
  kCurlyBracketRight,
  kSquareBracketLeft,
  kSquareBracketRight,
  kInt,
  kIs,
  kEnd,
  kIdent,
  kIntConst,
  kEndOfProgram,
};

std::ostream& operator<<(std::ostream& os, const DomainTag tag);

class Token {
 public:
  virtual ~Token() = default;

  DomainTag get_tag() const noexcept { return tag_; }
  const Fragment& get_coords() const& noexcept { return coords_; }

 protected:
  Token(const DomainTag tag, const Fragment& coords) noexcept
      : tag_(tag), coords_(coords) {}

  DomainTag tag_;
  Fragment coords_;
};

class IdentToken final : public Token {
  std::size_t code_;

 public:
  IdentToken(const std::size_t code, const Fragment& coords) noexcept
      : Token(DomainTag::kIdent, coords), code_(code) {}

  std::size_t get_code() const noexcept { return code_; }
};

class IntConstToken final : public Token {
  std::int64_t value_;

 public:
  IntConstToken(const std::int64_t value, const Fragment& coords) noexcept
      : Token(DomainTag::kIntConst, coords), value_(value) {}

  std::int64_t get_value() const noexcept { return value_; }
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

}  // namespace lexer
