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

std::ostream& operator<<(std::ostream& os, const DomainTag tag);

class Token {
 public:
  virtual ~Token() {}

  virtual void OutputAttr(std::ostream& os) const = 0;

  DomainTag tag() const noexcept { return tag_; }
  const Fragment& coords() const& noexcept { return coords_; }

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

  void OutputAttr(std::ostream& os) const override { os << str_; }
};

class TerminalToken final : public Token {
  std::string str_;

 public:
  template <typename String>
  TerminalToken(String&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kTerminal, coords), str_(std::forward<String>(str)) {}

  const std::string& get_str() const& noexcept { return str_; }

  void OutputAttr(std::ostream& os) const override { os << str_; }
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}

  void OutputAttr(std::ostream&) const override {}
};

std::ostream& operator<<(std::ostream& os, const Token& token);

}  // namespace lexer
