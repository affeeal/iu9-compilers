#include "symbol_table.h"

#include <cassert>

namespace parser {

namespace ast {

const Nonterminal* SymbolTable::FindNonterminal(const std::string& name) const {
  if (const auto it = nonterminals_.find(name); it != nonterminals_.cend()) {
    return it->second.get();
  }
  return nullptr;
}

const Nonterminal* SymbolTable::AddNonterminal(const std::string& name) {
  const auto [it, is_inserted] =
      nonterminals_.insert({name, std::make_unique<Nonterminal>(name)});
  assert(is_inserted);
  return it->second.get();
}

const Nonterminal* SymbolTable::GetNonterminal(const std::string& name) {
  if (const auto* const nonterminal = FindNonterminal(name)) {
    return nonterminal;
  }
  return AddNonterminal(name);
}

const Terminal* SymbolTable::FindTerminal(const std::string& name) const {
  if (const auto it = terminals_.find(name); it != terminals_.cend()) {
    return it->second.get();
  }
  return nullptr;
}

const Terminal* SymbolTable::AddTerminal(const std::string& name) {
  const auto [it, is_inserted] =
      terminals_.insert({name, std::make_unique<Terminal>(name)});
  assert(is_inserted);
  return it->second.get();
}

const Terminal* SymbolTable::GetTerminal(const std::string& name) {
  if (const auto* const terminal = FindTerminal(name)) {
    return terminal;
  }
  return AddTerminal(name);
}

}  // namespace ast

}  // namespace parser
