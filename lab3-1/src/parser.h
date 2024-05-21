#pragma once

#include <unordered_map>

// clang-format off
#include <boost/functional/hash.hpp>
// clang-format on

#include "node.h"
#include "scanner.h"
#include "token.h"

namespace parser {

class SententionalForm final {
 public:
  SententionalForm(std::initializer_list<dt::Symbol> il) : symbols_(il) {}

  auto Crbegin() const& noexcept { return symbols_.crbegin(); }
  auto Crend() const& noexcept { return symbols_.crend(); }

 private:
  std::vector<dt::Symbol> symbols_;
};

class AnalyzerTable final {
 public:
  static const AnalyzerTable& Instance();

  AnalyzerTable(const AnalyzerTable&) = delete;
  AnalyzerTable& operator=(const AnalyzerTable&) = delete;

  auto Find(const NonTerminal non_terminal, const lexer::DomainTag tag) const&;
  auto Cend() const& noexcept { return um_.cend(); }

 private:
  using Key = std::pair<NonTerminal, lexer::DomainTag>;
  using SententionalFormRef = std::reference_wrapper<const SententionalForm>;

  AnalyzerTable();

  std::vector<SententionalForm> sfs_;
  std::unordered_map<Key, SententionalFormRef, boost::hash<Key>> um_;
};

class Parser final {
 public:
  std::unique_ptr<dt::INode> TopDownParse(lexer::IScanner& scanner);

 private:
  const AnalyzerTable& table_ = AnalyzerTable::Instance();
};

}  // namespace parser
