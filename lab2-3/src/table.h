#pragma once

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include "node.h"
#include "scanner.h"
#include "token.h"

namespace parser {

using Symbol = std::variant<NonTerminal, lexer::DomainTag>;

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
  using SFRef = std::reference_wrapper<const SententionalForm>;
  using Data = std::unordered_map<Key, SFRef, boost::hash<Key>>;

  static const AnalyzerTable& Instance();

  AnalyzerTable(const AnalyzerTable&) = delete;
  AnalyzerTable& operator=(const AnalyzerTable&) = delete;

  auto Find(const NonTerminal nt, const lexer::DomainTag t) const&;
  auto Cend() const& noexcept;

 private:
  AnalyzerTable();

  std::vector<SententionalForm> sfs_;
  Data data_;
};

std::unique_ptr<Node> TopDownParse(lexer::Scanner& scanner,
                                   const AnalyzerTable& table);

}  // namespace parser
