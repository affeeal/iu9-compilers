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
  Token(const DomainTag tag, const Position& starting,
        const Position& following) noexcept
      : tag_(tag), coords_(starting, following) {}

  DomainTag tag_;
  Fragment coords_;
};

class SubstanceToken final : public Token {
 public:
  SubstanceToken(const std::string& str, const Position& starting,
                 const Position& following) noexcept
      : Token(DomainTag::kSubstance, starting, following), str_(str) {}

  const std::string& get_str() const& noexcept { return str_; }

 private:
  std::string str_;
};

class CoefficientToken final : public Token {
 public:
  CoefficientToken(const std::int64_t value, const Position& starting,
                   const Position& following) noexcept
      : Token(DomainTag::kCoefficient, starting, following), value_(value) {}

  std::int64_t get_value() const noexcept { return value_; }

 private:
  std::int64_t value_;
};

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Position& starting,
            const Position& following) noexcept
      : Token(tag, starting, following) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token);

}  // namespace lexer
