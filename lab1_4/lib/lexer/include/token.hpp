#pragma once

#include "compiler.hpp"
#include "fragment.hpp"
#include "position.hpp"

namespace lexer {

class Compiler;

enum class DomainTag {
  kComment,
  kEndOfProgram,
  kExclamatory,
  kExclamatoryTilda,
  kExist,
  kExit,
  kIdentifier,
  kNotFinal,
  kNumber,
  kWhitespace,
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

class IdentToken final : public Token {
 public:
  IdentToken(const std::size_t code, const Position& starting,
             const Position& following) noexcept
      : Token(DomainTag::kIdentifier, starting, following), code_(code) {}

  std::size_t get_code() const noexcept { return code_; }

 private:
  std::size_t code_;
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

class SpecToken final : public Token {
 public:
  SpecToken(const DomainTag tag, const Position& starting,
            const Position& following) noexcept
      : Token(tag, starting, following) {}

  SpecToken(const DomainTag tag, const Position& starting) noexcept
      : Token(tag, starting, starting) {}
};

void OutputToken(std::ostream& os, const Token* const token,
                 const Compiler& compiler);

}  // namespace lexer
