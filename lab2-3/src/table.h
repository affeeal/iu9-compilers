#pragma once

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include "node.h"
#include "token.h"

namespace parser {

struct Symbol final {
  std::variant<NonTerminal, lexer::DomainTag> data;

  bool IsTerminal() const noexcept {
    return std::holds_alternative<lexer::DomainTag>(data);
  }
};

class SententionalForm final {
 public:
  SententionalForm(std::initializer_list<Symbol> il) : symbols_(il) {}

  auto Crbegin() const& noexcept { return symbols_.crbegin(); }
  auto Crend() const& noexcept { return symbols_.crend(); }

 private:
  std::vector<Symbol> symbols_;
};

class AnalyzerTable final {
 public:
  using Key = std::pair<NonTerminal, lexer::DomainTag>;

  static const AnalyzerTable& Instance();

  AnalyzerTable(const AnalyzerTable&) = delete;
  AnalyzerTable& operator=(const AnalyzerTable&) = delete;

  // Table operations

 private:
  using SFRef = std::reference_wrapper<const SententionalForm>;
  using KeyToSFRef = std::unordered_map<Key, SFRef, boost::hash<Key>>;

  AnalyzerTable();

  std::vector<SententionalForm> sfs_;
  KeyToSFRef data_;
};

}  // namespace parser
