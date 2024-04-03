#pragma once

#include "fragment.h"

namespace lexer {

enum class DomainTag {
  kN,
  kPlus,
  kStar,
  kLparen,
  kRparen,
  kEndOfProgram,
};

std::ostream& operator<<(std::ostream& os, const DomainTag tag);

struct Token {
  DomainTag get_tag() const noexcept { return tag_; }
  const Fragment& get_coords() const& noexcept { return coords_; }

  virtual ~Token() {}

 protected:
  Token(const DomainTag tag, const Fragment& coords) noexcept
      : tag_(tag), coords_(coords) {}

  DomainTag tag_;
  Fragment coords_;
};

struct SpecToken final : Token {
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token);

}  // namespace lexer
