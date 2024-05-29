#pragma once

// clang-format off
#include <boost/unordered_set.hpp>
// clang-format on

namespace parser {

class Symbol final {
 public:
  enum class Type {
    kTerminal,
    kNonterminal,
    kSpecial,
  };

 public:
  Symbol(std::string name, const Type type) noexcept
      : name_(std::move(name)), type_(type) {}

  bool operator==(const Symbol&) const = default;

  const std::string& get_name() const noexcept { return name_; }
  Type get_type() const noexcept { return type_; }

 private:
  std::string name_;
  Type type_;
};

const auto kEpsilon = Symbol{"ε", Symbol::Type::kSpecial};
const auto kEndOfProgram = Symbol{"END_OF_PROGRAM", Symbol::Type::kTerminal};

std::size_t hash_value(const Symbol& symbol);

using SymbolVecIter = std::vector<Symbol>::const_iterator;
using SymbolSetIter = boost::unordered_set<Symbol>::const_iterator;

}  // namespace parser
