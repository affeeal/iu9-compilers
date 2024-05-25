#pragma once

#include <string>
#include <unordered_map>

#include "ast.h"

namespace parser {

namespace ast {

class SymbolTable final {
  std::unordered_map<std::string, std::unique_ptr<NonterminalSymbol>>
      nonterminals_;
  std::unordered_map<std::string, std::unique_ptr<TerminalSymbol>> terminals_;
  std::unique_ptr<SpecialSymbol> epsilon_, dollar_;

 public:
  SymbolTable()
      : epsilon_(std::make_unique<SpecialSymbol>()),
        dollar_(std::make_unique<SpecialSymbol>()) {}

  const SpecialSymbol* GetEpsilon() const noexcept { return epsilon_.get(); }
  const SpecialSymbol* GetDollar() const noexcept { return dollar_.get(); }

  const NonterminalSymbol* FindNonterminal(const std::string& name) const;
  const NonterminalSymbol* AddNonterminal(const std::string& name);
  const NonterminalSymbol* GetNonterminal(const std::string& name);

  const TerminalSymbol* FindTerminal(const std::string& name) const;
  const TerminalSymbol* AddTerminal(const std::string& name);
  const TerminalSymbol* GetTerminal(const std::string& name);
};

}  // namespace ast

}  // namespace parser
