#pragma once

#include "fragment.hpp"

namespace lexer {

enum class DomainTag {
  kArrow,
  kEndOfProgram,
  kCoefficient,
  kPlus,
  kSubstance,
};

std::ostream& operator<<(std::ostream& os, const DomainTag tag);

class Token {
 public:
  DomainTag get_tag() const noexcept { return tag_; }
  const Fragment& get_coords() const& noexcept { return coords_; }

  virtual ~Token() {}

 protected:
  Token(const DomainTag tag, const Fragment& coords) noexcept
      : tag_(tag), coords_(coords) {}

  DomainTag tag_;
  Fragment coords_;
};

class SubstanceToken final : public Token {
 public:
  SubstanceToken(const std::string& str, const Fragment& coords) noexcept
      : Token(DomainTag::kSubstance, coords), str_(str) {}

  SubstanceToken(std::string&& str, const Fragment& coords) noexcept
      : Token(DomainTag::kSubstance, coords), str_(std::move(str)) {}

  const std::string& get_str() const& noexcept { return str_; }

 private:
  std::string str_;
};

class CoefficientToken final : public Token {
 public:
  CoefficientToken(const std::int64_t value, const Fragment& coords) noexcept
      : Token(DomainTag::kCoefficient, coords), value_(value) {}

  std::int64_t get_value() const noexcept { return value_; }

 private:
  std::int64_t value_;
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Fragment& coords) noexcept
      : Token(tag, coords) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token);

}  // namespace lexer
