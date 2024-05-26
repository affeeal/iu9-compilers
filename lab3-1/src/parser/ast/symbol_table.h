#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ast.h"

namespace parser {

namespace ast {

class SymbolTable final {
  std::unordered_map<std::string, std::unique_ptr<Nonterminal>> nonterminals_;
  std::unordered_map<std::string, std::unique_ptr<Terminal>> terminals_;
  std::unique_ptr<Special> epsilon_, dollar_;

 public:
  SymbolTable()
      : epsilon_(std::make_unique<Special>()),
        dollar_(std::make_unique<Special>()) {}

  const Special* GetEpsilon() const noexcept { return epsilon_.get(); }
  const Special* GetDollar() const noexcept { return dollar_.get(); }

  const Nonterminal* FindNonterminal(const std::string& name) const;
  const Nonterminal* AddNonterminal(const std::string& name);
  const Nonterminal* GetNonterminal(const std::string& name);

  const Terminal* FindTerminal(const std::string& name) const;
  const Terminal* AddTerminal(const std::string& name);
  const Terminal* GetTerminal(const std::string& name);
};

}  // namespace ast

}  // namespace parser
