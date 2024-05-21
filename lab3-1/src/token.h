#pragma once

#include "fragment.h"

namespace lexer {

enum class DomainTag {
  kNonTerminal,
  kTerminal,
  kOpArrow,
  kKwAxiom,
  kKwEpsilon,
  kKwOr,
  kKwEnd,
  kEndOfProgram,
};

std::string_view ToString(const DomainTag tag);
std::ostream& operator<<(std::ostream& os, const DomainTag tag);

class Token {
 public:
  virtual ~Token() = default;

  DomainTag tag() const noexcept { return tag_; }
  const Fragment& coords() const noexcept { return coords_; }

 protected:
  Token(const DomainTag tag, const Fragment& coords) noexcept
      : tag_(tag), coords_(coords) {}

  DomainTag tag_;
  Fragment coords_;
};

class NonTerminalToken final : public Token {
  std::string str_;

 public:
  template <typename String>
  NonTerminalToken(String&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kNonTerminal, coords),
        str_(std::forward<String>(str)) {}

  const std::string& get_str() const& noexcept { return str_; }
};

class TerminalToken final : public Token {
  std::string str_;

 public:
  template <typename String>
  TerminalToken(String&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kTerminal, coords), str_(std::forward<String>(str)) {}

  const std::string& get_str() const& noexcept { return str_; }
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

}  // namespace lexer
