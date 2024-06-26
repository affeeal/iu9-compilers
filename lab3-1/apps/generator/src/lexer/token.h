#pragma once

#include "fragment.h"

namespace lexer {

enum class DomainTag {
  kNonterminal,
  kTerminal,
  kArrow,
  kKwAxiom,
  kKwEpsilon,
  kKwOr,
  kKwEnd,
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

class NonterminalToken final : public Token {
  std::string str_;

 public:
  template <typename String>
  NonterminalToken(String&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kNonterminal, coords),
        str_(std::forward<String>(str)) {}

  const std::string& get_str() const noexcept { return str_; }
};

class TerminalToken final : public Token {
  std::string str_;

 public:
  template <typename String>
  TerminalToken(String&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kTerminal, coords), str_(std::forward<String>(str)) {}

  const std::string& get_str() const noexcept { return str_; }
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

}  // namespace lexer
